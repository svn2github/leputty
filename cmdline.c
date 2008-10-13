/*
 * cmdline.c - command-line parsing shared between many of the
 * PuTTY applications
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "putty.h"

/*
 * Some command-line parameters need to be saved up until after
 * we've loaded the saved session which will form the basis of our
 * eventual running configuration. For this we use the macro
 * SAVEABLE, which notices if the `need_save' parameter is set and
 * saves the parameter and value on a list.
 * 
 * We also assign priorities to saved parameters, just to slightly
 * ameliorate silly ordering problems. For example, if you specify
 * a saved session to load, it will be loaded _before_ all your
 * local modifications such as -L are evaluated; and if you specify
 * a protocol and a port, the protocol is set up first so that the
 * port can override its choice of port number.
 * 
 * (In fact -load is not saved at all, since in at least Plink the
 * processing of further command-line options depends on whether or
 * not the loaded session contained a hostname. So it must be
 * executed immediately.)
 */

#define NPRIORITIES 2

struct cmdline_saved_param {
    char *p, *value;
};
struct cmdline_saved_param_set {
    struct cmdline_saved_param *params;
    int nsaved, savesize;
};

/*
 * C guarantees this structure will be initialised to all zero at
 * program start, which is exactly what we want.
 */
static struct cmdline_saved_param_set saves[NPRIORITIES];

static void cmdline_save_param(char *p, char *value, int pri)
{
    if (saves[pri].nsaved >= saves[pri].savesize) {
	saves[pri].savesize = saves[pri].nsaved + 32;
	saves[pri].params = sresize(saves[pri].params, saves[pri].savesize,
				    struct cmdline_saved_param);
    }
    saves[pri].params[saves[pri].nsaved].p = p;
    saves[pri].params[saves[pri].nsaved].value = value;
    saves[pri].nsaved++;
}

void cmdline_cleanup(void)
{
    int pri;

    for (pri = 0; pri < NPRIORITIES; pri++)
	sfree(saves[pri].params);
}

#define SAVEABLE(pri) do { \
    if (need_save) { cmdline_save_param(p, value, pri); return ret; } \
} while (0)

static char *cmdline_password = NULL;

/*
 * Similar interface to get_userpass_input(), except that here a -1
 * return means that we aren't capable of processing the prompt and
 * someone else should do it.
 */
int cmdline_get_passwd_input(prompts_t *p, unsigned char *in, int inlen) {

    static int tried_once = 0;

    /*
     * We only handle prompts which don't echo (which we assume to be
     * passwords), and (currently) we only cope with a password prompt
     * that comes in a prompt-set on its own.
     */
    if (!cmdline_password || in || p->n_prompts != 1 || p->prompts[0]->echo) {
	return -1;
    }

    /*
     * If we've tried once, return utter failure (no more passwords left
     * to try).
     */
    if (tried_once)
	return 0;

    strncpy(p->prompts[0]->result, cmdline_password,
	    p->prompts[0]->result_len);
    p->prompts[0]->result[p->prompts[0]->result_len-1] = '\0';
    memset(cmdline_password, 0, strlen(cmdline_password));
    tried_once = 1;
    return 1;

}

void cmdline_setautopasswd(char *passwd)
{
	cmdline_password = passwd;
}

/*
 * Here we have a flags word which describes the capabilities of
 * the particular tool on whose behalf we're running. We will
 * refuse certain command-line options if a particular tool
 * inherently can't do anything sensible. For example, the file
 * transfer tools (psftp, pscp) can't do a great deal with protocol
 * selections (ever tried running scp over telnet?) or with port
 * forwarding (even if it wasn't a hideously bad idea, they don't
 * have the select() infrastructure to make them work).
 */
int cmdline_tooltype = 0;

static int cmdline_check_unavailable(int flag, char *p)
{
    if (cmdline_tooltype & flag) {
	if (p) {
	cmdline_error("option \"%s\" not available in this tool", p);
	}
	return 1;
    }
    return 0;
}

#define UNAVAILABLE_IN(flag) do { \
    if (cmdline_check_unavailable(flag, p)) return ret; \
} while (0)

#define PRINTS(s, c) do {\
    if (!cmdline_check_unavailable(c, NULL)) {\
        printf(s);\
    }\
} while(0)

/*
 * Process a standard command-line parameter. `p' is the parameter
 * in question; `value' is the subsequent element of argv, which
 * may or may not be required as an operand to the parameter.
 * If `need_save' is 1, arguments which need to be saved as
 * described at this top of this file are, for later execution;
 * if 0, they are processed normally. (-1 is a special value used
 * by pterm to count arguments for a preliminary pass through the
 * argument list; it causes immediate return with an appropriate
 * value with no action taken.)
 * Return value is 2 if both arguments were used; 1 if only p was
 * used; 0 if the parameter wasn't one we recognised; -2 if it
 * should have been 2 but value was NULL.
 */

#define RETURN(x) do { \
    if ((x) == 2 && !value) return -2; \
    ret = x; \
    if (need_save < 0) return x; \
} while (0)

int cmdline_process_param(char *p, char *value, int need_save, Config *cfg)
{
    int ret = 0;

    if (!strcmp(p, "-load")) {
	RETURN(2);
	/* This parameter must be processed immediately rather than being
	 * saved. */
	do_defaults(value, cfg);
	loaded_session = TRUE;
	return 2;
    }
    if (!strcmp(p, "-ssh")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	default_protocol = cfg->protocol = PROT_SSH;
	default_port = cfg->port = 22;
	return 1;
    }
    if (!strcmp(p, "-telnet")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	default_protocol = cfg->protocol = PROT_TELNET;
	default_port = cfg->port = 23;
	return 1;
    }
    if (!strcmp(p, "-rlogin")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	default_protocol = cfg->protocol = PROT_RLOGIN;
	default_port = cfg->port = 513;
	return 1;
    }
    if (!strcmp(p, "-raw")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	default_protocol = cfg->protocol = PROT_RAW;
    }
    if (!strcmp(p, "-v")) {
	RETURN(1);
	flags |= FLAG_VERBOSE;
    }
    if (!strcmp(p, "-l")) {
	char *passwd;
	RETURN(2);
	UNAVAILABLE_IN(TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	passwd = strchr(value, '%');
	if (passwd) {
		*passwd = 0;
		passwd++;

		cmdline_setautopasswd(passwd);
	}
	strncpy(cfg->username, value, sizeof(cfg->username));
	cfg->username[sizeof(cfg->username) - 1] = '\0';
    }
    if ((!strcmp(p, "-L") || !strcmp(p, "-R") || !strcmp(p, "-D"))) {
	char *fwd, *ptr, *q, *qq;
	int dynamic, i=0;
	RETURN(2);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	dynamic = !strcmp(p, "-D");
	fwd = value;
	ptr = cfg->portfwd;
	/* if existing forwards, find end of list */
	while (*ptr) {
	    while (*ptr)
		ptr++;
	    ptr++;
	}
	i = ptr - cfg->portfwd;
	ptr[0] = p[1];  /* insert a 'L', 'R' or 'D' at the start */
	ptr++;
	if (1 + strlen(fwd) + 2 > sizeof(cfg->portfwd) - i) {
	    cmdline_error("out of space for port forwardings");
	    return ret;
	}
	strncpy(ptr, fwd, sizeof(cfg->portfwd) - i - 2);
	if (!dynamic) {
	    /*
	     * We expect _at least_ two colons in this string. The
	     * possible formats are `sourceport:desthost:destport',
	     * or `sourceip:sourceport:desthost:destport' if you're
	     * specifying a particular loopback address. We need to
	     * replace the one between source and dest with a \t;
	     * this means we must find the second-to-last colon in
	     * the string.
	     */
	    q = qq = strchr(ptr, ':');
	    while (qq) {
		char *qqq = strchr(qq+1, ':');
		if (qqq)
		    q = qq;
		qq = qqq;
	    }
	    if (q) *q = '\t';	       /* replace second-last colon with \t */
	}
	cfg->portfwd[sizeof(cfg->portfwd) - 1] = '\0';
	cfg->portfwd[sizeof(cfg->portfwd) - 2] = '\0';
	ptr[strlen(ptr)+1] = '\000';    /* append 2nd '\000' */
    }
    if ((!strcmp(p, "-nc"))) {
	char *host, *portp;

	RETURN(2);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);

	host = portp = value;
	while (*portp && *portp != ':')
	    portp++;
	if (*portp) {
	    unsigned len = portp - host;
	    if (len >= sizeof(cfg->ssh_nc_host))
		len = sizeof(cfg->ssh_nc_host) - 1;
	    strncpy(cfg->ssh_nc_host, value, len);
	    cfg->ssh_nc_host[sizeof(cfg->ssh_nc_host) - 1] = '\0';
	    cfg->ssh_nc_port = atoi(portp+1);
	} else {
	    cmdline_error("-nc expects argument of form 'host:port'");
	    return ret;
	}
    }
    if (!strcmp(p, "-m")) {
	char *filename, *command;
	int cmdlen, cmdsize;
	FILE *fp;
	int c, d;

	RETURN(2);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);

	filename = value;

	cmdlen = cmdsize = 0;
	command = NULL;
	fp = fopen(filename, "r");
	if (!fp) {
	    cmdline_error("unable to open command "
			  "file \"%s\"", filename);
	    return ret;
	}
	do {
	    c = fgetc(fp);
	    d = c;
	    if (c == EOF)
		d = 0;
	    if (cmdlen >= cmdsize) {
		cmdsize = cmdlen + 512;
		command = sresize(command, cmdsize, char);
	    }
	    command[cmdlen++] = d;
	} while (c != EOF);
	cfg->remote_cmd_ptr = command;
	cfg->remote_cmd_ptr2 = NULL;
	cfg->nopty = TRUE;      /* command => no terminal */
    }
    if (!strcmp(p, "-P")) {
	RETURN(2);
	UNAVAILABLE_IN(TOOLTYPE_NONNETWORK);
	SAVEABLE(1);		       /* lower priority than -ssh,-telnet */
	cfg->port = atoi(value);
    }
    if (!strcmp(p, "-pw")) {
	RETURN(2);
	cmdline_setautopasswd(value);
	UNAVAILABLE_IN(TOOLTYPE_NONNETWORK);
    }

    if (!strcmp(p, "-agent") || !strcmp(p, "-pagent") ||
	!strcmp(p, "-pageant")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_NONNETWORK);
	cfg->tryagent = TRUE;
    }
    if (!strcmp(p, "-noagent") || !strcmp(p, "-nopagent") ||
	!strcmp(p, "-nopageant")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_NONNETWORK);
	cfg->tryagent = FALSE;
    }

    if (!strcmp(p, "-A")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	cfg->agentfwd = 1;
    }
    if (!strcmp(p, "-a")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	cfg->agentfwd = 0;
    }

    if (!strcmp(p, "-X")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	cfg->x11_forward = 1;
    }
    if (!strcmp(p, "-x")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	cfg->x11_forward = 0;
    }

    if (!strcmp(p, "-t")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	cfg->nopty = 0;
    }
    if (!strcmp(p, "-T")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	cfg->nopty = 1;
    }

    if (!strcmp(p, "-N")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER | TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	cfg->ssh_no_shell = 1;
    }

    if (!strcmp(p, "-C")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	cfg->compression = 1;
    }

    if (!strcmp(p, "-1")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	default_protocol = cfg->protocol = PROT_SSH;
	default_port = cfg->port = 22;
	cfg->sshprot = 0;	       /* ssh protocol 1 only */
    }
    if (!strcmp(p, "-2")) {
	RETURN(1);
	UNAVAILABLE_IN(TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	default_protocol = cfg->protocol = PROT_SSH;
	default_port = cfg->port = 22;
	cfg->sshprot = 3;	       /* ssh protocol 2 only */
    }

    if (!strcmp(p, "-i")) {
	RETURN(2);
	UNAVAILABLE_IN(TOOLTYPE_NONNETWORK);
	SAVEABLE(0);
	cfg->keyfile = filename_from_str(value);
    }

    if (!strcmp(p, "-4") || !strcmp(p, "-ipv4")) {
	RETURN(1);
	SAVEABLE(1);
	cfg->addressfamily = ADDRTYPE_IPV4;
    }
    if (!strcmp(p, "-6") || !strcmp(p, "-ipv6")) {
	RETURN(1);
	SAVEABLE(1);
	cfg->addressfamily = ADDRTYPE_IPV6;
    }
   if (!strcmp(p, "-proxy")) {
	char *proto, *hostname, *port;
	RETURN(2);
	SAVEABLE(1);
	// proxy format:
	// proto:host:port
	// proto is HTTP SOCKS TELNET
	proto = value;
	hostname=strchr(proto, ':');
	if (hostname) {
		*hostname++ = '\0';
		port = strchr(hostname, ':');
		if (port) *port++ = '\0';
	} else {
		port = NULL;
	}
	if (!stricmp(proto, "HTTP"))
		cfg->proxy_type = PROXY_HTTP;
	else if (!stricmp(proto, "SOCKS") || !stricmp(proto, "SOCKS5")) {
		cfg->proxy_type = PROXY_SOCKS5;
	}
	else if (!stricmp(proto, "SOCKS4")) {
		cfg->proxy_type = PROXY_SOCKS4;
	}

	else if (!stricmp(proto, "TELNET"))
		cfg->proxy_type = PROXY_TELNET ;
	else {
		cmdline_error("Invalid proxy type %s", proto);
		return ret;
	}
	if (port) cfg->proxy_port = atoi(port);
	if (!hostname || !*hostname || !port || !cfg->proxy_port) {
		cmdline_error("Invalid proxy host/port");
		return ret;
	}
	strncpy(cfg->proxy_host, hostname, sizeof(cfg->proxy_host) - 1);
	cfg->proxy_host[sizeof(cfg->proxy_host) - 1] = '\0';
    }

    if (!strcmp(p, "-proxyl")) {
	RETURN(2);
	SAVEABLE(2);
	if (cfg->proxy_type == PROXY_NONE) {
		cmdline_error("No proxy specified with username/password");
		return ret;
	}
	strncpy(cfg->proxy_username, value, sizeof(cfg->proxy_username) - 1);
	cfg->proxy_username[sizeof(cfg->proxy_username) - 1] = '\0';
    }

    if (!strcmp(p, "-proxypw")) {
	RETURN(2);
	SAVEABLE(2);
	if (cfg->proxy_type == PROXY_NONE) {
		cmdline_error("No proxy specified with username/password");
		return ret;
	}
	strncpy(cfg->proxy_password, value, sizeof(cfg->proxy_password) - 1);
	cfg->proxy_password[sizeof(cfg->proxy_password) - 1] = '\0';
    }

    if (!strcmp(p , "-proxytelnetcommand")) {
    	RETURN(2);
	SAVEABLE(2);
	if (cfg->proxy_type != PROXY_TELNET) {
		cmdline_error("Proxy must be a TELNET proxy for telnetcommand to work");
		return ret;
	}
	strncpy(cfg->proxy_telnet_command, value, sizeof(cfg->proxy_telnet_command) - 1);
	cfg->proxy_telnet_command[sizeof(cfg->proxy_telnet_command) - 1] = '\0';
    }

    if (!strcmp(p, "-proxyexcludelist")) {
	RETURN(2);
	SAVEABLE(2);
	if (cfg->proxy_type == PROXY_NONE) {
		cmdline_error("No proxy specified with username/password");
		return ret;
	}
	strncpy(cfg->proxy_exclude_list, value, sizeof(cfg->proxy_exclude_list) -1);
	cfg->proxy_exclude_list[sizeof(cfg->proxy_exclude_list) - 1] = '\0';
    }
    if (!strcmp(p, "-keepalive")) {
	RETURN(2);
	UNAVAILABLE_IN(TOOLTYPE_FILETRANSFER);
	SAVEABLE(1);
	cfg->ping_interval = atoi(value);
    }

    return ret;			       /* unrecognised */
}

void printparams()
{
	printf("  -V             print version information and exit\n");
    printf("  -pgpfp         print PGP key fingerprints and exit\n");
    printf("  -v             show verbose messages\n");
    printf("  -load sessname Load settings from saved session\n");
    PRINTS("  -ssh -telnet -rlogin -raw\n", TOOLTYPE_FILETRANSFER); 
    PRINTS("                 force use of a particular protocol (default SSH)\n", TOOLTYPE_FILETRANSFER); 
    printf("  -P port        connect to specified port\n");
    printf("  -l user        connect with specified username\n");
    
    PRINTS("  -keepalive t   sends a keepalive packet every t seconds\n", TOOLTYPE_FILETRANSFER);

    printf("The following options only apply to SSH connections:\n");
    printf("  -pw passw      login with specified password\n");
    PRINTS("  -D [listen-IP:]listen-port\n", TOOLTYPE_FILETRANSFER);
    PRINTS("                 Dynamic SOCKS-based port forwarding\n", TOOLTYPE_FILETRANSFER);
    PRINTS("  -L [listen-IP:]listen-port:host:port\n", TOOLTYPE_FILETRANSFER);
    PRINTS("                 Forward local port to remote address\n", TOOLTYPE_FILETRANSFER);
    PRINTS("  -R [listen-IP:]listen-port:host:port\n", TOOLTYPE_FILETRANSFER);
    PRINTS("                 Forward remote port to local address\n", TOOLTYPE_FILETRANSFER);
    PRINTS("  -X -x          enable / disable X11 forwarding\n", TOOLTYPE_FILETRANSFER); 
    PRINTS("  -A -a          enable / disable agent forwarding\n", TOOLTYPE_FILETRANSFER); 
    PRINTS("  -t -T          enable / disable pty allocation\n", TOOLTYPE_FILETRANSFER); 
    printf("  -1 -2          force use of particular protocol version\n"); 
    printf("  -4 -6          force use of IPv4 or IPv6\n");
    printf("  -C             enable compression\n"); 
    printf("  -i key         private key file for authentication\n"); 
	PRINTS("  -m file        read remote command(s) from file\n", TOOLTYPE_FILETRANSFER);
	printf("  -N             don't start a shell/command (SSH-2 only)\n");
    printf("Proxy support options:\n");
    printf("  -proxy TYPE:HOSTNAME:PORT connects through a proxy. TYPE is one of HTTP,SOCKS,SOCKS4,SOCKS5,TELNET\n");
    printf("  -proxyl loginname            logs in to proxy with loginname\n");
    printf("  -proxypw passw               logs in to proxy with specified password\n");
    printf("  -proxytelnetcommand command  sets the telnet command to use with telnet proxy\n");
    printf("  -proxyexcludelist list       use the specified exclude list to proxy\n");
}

void cmdline_run_saved(Config *cfg)
{
    int pri, i;
    for (pri = 0; pri < NPRIORITIES; pri++)
	for (i = 0; i < saves[pri].nsaved; i++)
	    cmdline_process_param(saves[pri].params[i].p,
				  saves[pri].params[i].value, 0, cfg);
}
