#define RETSIGTYPE void
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <pcap.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "Pack.h"
#include "socket/socklib.h"

#ifndef setsignal_h
#define setsignal_h

#define merror printf
#define mwarning printf

RETSIGTYPE (*setsignal(int, RETSIGTYPE (*)(int)))(int);
#endif

Socket mSocket;

char cpre580f98[] = "netdump";

void raw_print(u_char *user, const struct pcap_pkthdr *h, const u_char *p);

int packettype;

char *program_name;

/* Externs */
extern void bpf_dump(const struct bpf_program *, int);	// what the declaration of function bpf_dump()?

extern char *copy_argv(char **);					// refer to "util.c"

/* Forwards */
 void program_ending(int);
void init_sender();

/* Length of saved portion of packet. */
int snaplen = 1500;;

static pcap_t *pd;	// what is the exact structure of "pcap_t"? (refer to "pcap\pcap.h": typedef struct pcap pcap_t;	also refer to "pcap-int.h")
						//struct pcap {
						//	/*
						//	 * Method to call to read packets on a live capture.
						//	 */
						//	read_op_t read_op;		// typedef int	(*read_op_t)(pcap_t *, int cnt, pcap_handler, u_char *);

						//	/*
						//	 * Method to call to read to read packets from a savefile.
						//	 */
						//	int (*next_packet_op)(pcap_t *, struct pcap_pkthdr *, u_char **);

						//#ifdef WIN32
						//	ADAPTER *adapter;
						//	LPPACKET Packet;
						//	int nonblock;
						//#else
						//	int fd;
						//	int selectable_fd;
						//#endif /* WIN32 */

						//	/*
						//	 * Read buffer.
						//	 */
						//	int bufsize;
						//	u_char *buffer;
						//	u_char *bp;
						//	int cc;

						//	int break_loop;		/* flag set to force break from packet-reading loop */
						//	void *priv;			/* private data for methods */
						//	int swapped;
						//	FILE *rfile;		/* null if live capture, non-null if savefile */  (rfile = NULL if live capture.)
						//	int fddipad;
						//	struct pcap *next;	/* list of open pcaps that need stuff cleared on close */
						//	/*
						//	 * File version number; meaningful only for a savefile, but we
						//	 * keep it here so that apps that (mistakenly) ask for the
						//	 * version numbers will get the same zero values that they
						//	 * always did.
						//	 */
						//	int version_major;
						//	int version_minor;
						//	int snapshot;
						//	int linktype;		/* Network linktype */
						//	int linktype_ext;   /* Extended information stored in the linktype field of a file */
						//	int tzoff;			/* timezone offset */
						//	int offset;			/* offset for proper alignment */
						//	int activated;		/* true if the capture is really started */
						//	int oldstyle;		/* if we're opening with pcap_open_live() */

						/*
						 struct pcap_opt {
						 	char *source;
						 	int	timeout;	// timeout for buffering
						 	int	buffer_size;
						 	int	promisc;
						 	int	rfmon;		// monitor mode
						 	int	immediate;	// immediate mode - deliver packets as soon as they arrive
						 	int	tstamp_type;
						 	int	tstamp_precision;
						 };
						*/
						//	struct pcap_opt opt;		// refer to "pcap-int.h"

						//	/*
						//	 * Place holder for pcap_next().
						//	 */
						//	u_char *pkt;

						/*  refer to "pcap\pcap.h"
						 typedef enum {
						        PCAP_D_INOUT = 0,
						        PCAP_D_IN,			// 1
						        PCAP_D_OUT			// 2
						 } pcap_direction_t;
						*/
						//	/* We're accepting only packets in this direction/these directions. */
						//	pcap_direction_t direction;

						//	/*
						//	 * Flags to affect BPF code generation.
						//	 */
						//	int bpf_codegen_flags;

						//	/*
						//	 * Placeholder for filter code if bpf not in kernel.
						//	 */
						//	struct bpf_program fcode;		// struct bpf_program: refer to "bpf.h"

						//	char errbuf[PCAP_ERRBUF_SIZE + 1];		// #define PCAP_ERRBUF_SIZE 256		in "pcap\pcap.h"
						//	int dlt_count;
						//	u_int *dlt_list;
						//	int tstamp_type_count;
						//	u_int *tstamp_type_list;
						//	int tstamp_precision_count;
						//	u_int *tstamp_precision_list;

						/* refer to "pcap\pcap.h"
						 struct pcap_pkthdr {
						 	struct timeval ts;	// time stamp
						 	bpf_u_int32 caplen;	// length of portion present
						 	bpf_u_int32 len;	// length this packet (off wire)
						 };
						*/
						//	struct pcap_pkthdr pcap_header;	/* This is needed for the pcap_next_ex() to work */

						//	/*
						//	 * More methods.
						//	 */
						//	activate_op_t activate_op;				// refer to "pcap-int.h":	typedef int	(*activate_op_t)(pcap_t *);
						//	can_set_rfmon_op_t can_set_rfmon_op;	// refer to "pcap-int.h":	typedef int	(*can_set_rfmon_op_t)(pcap_t *);
						//	inject_op_t inject_op;					// refer to "pcap-int.h":	typedef int	(*inject_op_t)(pcap_t *, const void *, size_t);
						//	setfilter_op_t setfilter_op;			// refer to "pcap-int.h":	typedef int	(*setfilter_op_t)(pcap_t *, struct bpf_program *);
						//	setdirection_op_t setdirection_op;		// refer to "pcap-int.h":	typedef int	(*setdirection_op_t)(pcap_t *, pcap_direction_t);
						//	set_datalink_op_t set_datalink_op;		// refer to "pcap-int.h":	typedef int	(*set_datalink_op_t)(pcap_t *, int);
						//	getnonblock_op_t getnonblock_op;		// refer to "pcap-int.h":	typedef int	(*getnonblock_op_t)(pcap_t *, char *);
						//	setnonblock_op_t setnonblock_op;		// refer to "pcap-int.h":	typedef int	(*setnonblock_op_t)(pcap_t *, int, char *);
						//	stats_op_t stats_op;					// refer to "pcap-int.h":	typedef int	(*stats_op_t)(pcap_t *, struct pcap_stat *);

						//	/*
						//	 * Routine to use as callback for pcap_next()/pcap_next_ex().
						//	 */
						//	pcap_handler oneshot_callback;		// refer to "pcap\pcap.h": typedef void (*pcap_handler)(u_char *, const struct pcap_pkthdr *, const u_char *);

						//#ifdef WIN32
						//	/*
						//	 * These are, at least currently, specific to the Win32 NPF
						//	 * driver.
						//	 */
						//	setbuff_op_t setbuff_op;				// refer to "pcap-int.h":	typedef int	(*setbuff_op_t)(pcap_t *, int);
						//	setmode_op_t setmode_op;				// refer to "pcap-int.h":	typedef int	(*setmode_op_t)(pcap_t *, int);
						//	setmintocopy_op_t setmintocopy_op;		// refer to "pcap-int.h":	typedef int	(*setmintocopy_op_t)(pcap_t *, int);
						//	getadapter_op_t getadapter_op;			// refer to "pcap-int.h":	typedef Adapter *(*getadapter_op_t)(pcap_t *);
						//#endif
						//	cleanup_op_t cleanup_op;				// refer to "pcap-int.h":	typedef void	(*cleanup_op_t)(pcap_t *);
						//};

extern int optind;			// what is the meaning of "int optind"? (#include <unistd.h>)
extern int opterr;			// what is the meaning of "int opterr"? (#include <unistd.h>)
extern char *optarg;		// what is the meaning of "char *optarg"? (#include <unistd.h>)
int pflag = 0, aflag = 0;
int cnt;

int
main(int argc, char **argv)
{
    init_sender();

	int op, i, done = 0;
	bpf_u_int32 localnet, netmask;					// what is the exact structure of "bpf_u_int32"? (refer to "pcap\bpf.h": typedef u_int bpf_u_int32;)
	char *cp, *cmdbuf, *device;
	struct bpf_program fcode;		// what is the exact structure of "bpf_program"? (refer to "pcap\bpf.h": struct bpf_program { u_int bf_len; struct bpf_insn *bf_insns; };
									// 													struct bpf_insn { u_short	code;	u_char 	jt;		u_char 	jf;		bpf_u_int32 k; };
	 void (*oldhandler)(int);
	u_char *pcap_userdata;
	char ebuf[PCAP_ERRBUF_SIZE];					// what the declaration of "PCAP_ERRBUF_SIZE"? (refer to "pcap\pcap.h": #define PCAP_ERRBUF_SIZE 256)

	cnt = 0;
	device = NULL;

	if ((cp = strrchr(argv[0], '/')) != NULL)		// what is the function of "strrchr(char *, char)"?
		program_name = cp + 1;
	else
		program_name = argv[0];

	opterr = 0;
	while ((i = getopt(argc, argv, "pa")) != -1)	// what is the function of "getopt(int, char **, char *)"? (#include <unistd.h>)
	{
		switch (i)
		{
		case 'p':
			pflag = 1;		// what is the meaning of 'p' flag?
		break;
		case 'a':
			aflag = 1;		// what is the meaning of 'a' flag?
		break;
		case '?':
		default:
			done = 1;
		break;
		}
		if (done) break;
	}
	if (argc > (optind)) cmdbuf = copy_argv(&argv[optind]);				// refer to "util.c", concatenating arguments with spaces.
		else cmdbuf = (char *) "";

	if (device == NULL) {
		device = pcap_lookupdev(ebuf);				// what the declaration of function pcap_lookupdev(char *)? (refer to "pcap\pcap.h")
		if (device == NULL)
			merror("%s", ebuf);
	}
	pd = pcap_open_live(device, snaplen,  1, 1000, ebuf);	// what the declaration of function "pcap_open_live"? (refer to "pcap\pcap.h" and "pcap.c")
	if (pd == NULL)
		merror("%s", ebuf);
	i = pcap_snapshot(pd);				// what the declaration of function "pcap_snapshot(pcap_t *)"? (refer to "pcap.c")
										//int
										//pcap_snapshot(pcap_t *p)
										//{
										//	if (!p->activated)
										//		return (PCAP_ERROR_NOT_ACTIVATED);
										//	return (p->snapshot);
										//}
	if (snaplen < i) {
		mwarning("snaplen raised from %d to %d", snaplen, i);
		snaplen = i;
	}
	if (pcap_lookupnet(device, &localnet, &netmask, ebuf) < 0) {		// what the declaration of function "pcap_lookupnet"? (refer to "pcap\pcap.h")
		localnet = 0;
		netmask = 0;
		mwarning("%s", ebuf);
	}
	/*
	 * Let user own process after socket has been opened.
	 */
	setuid(getuid());								// what are the functions of "getuid()" and "setuid"? (#include <unistd.h>)

	if (pcap_compile(pd, &fcode, cmdbuf, 1, netmask) < 0)				// what the declaration of function "pcap_compile"? (refer to "pcap\pcap.h")
		merror("%s", pcap_geterr(pd));

	(void)setsignal(SIGTERM, program_ending);		// what are the SIGTERM, SIGINT, SIGHUP and SIG_DFL? (#include <signal.h>)
	(void)setsignal(SIGINT, program_ending);
	/* Cooperate with nohup(1) */
	if ((oldhandler = setsignal(SIGHUP, program_ending)) != SIG_DFL)
		(void)setsignal(SIGHUP, oldhandler);

	if (pcap_setfilter(pd, &fcode) < 0)				// what the declaration of function "pcap_setfilter"? (refer to "pcap.c")
													//int
													//pcap_setfilter(pcap_t *p, struct bpf_program *fp)
													//{
													//	return (p->setfilter_op(p, fp));
													//}
		merror("%s", pcap_geterr(pd));
	pcap_userdata = 0;
	(void)fprintf(stderr, "%s: listening on %s\n", program_name, device);
	if (pcap_loop(pd, cnt, raw_print, pcap_userdata) < 0) {	// what the declaration of function "pcap_loop"? (refer to "pcap.c")
															//int
															//pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user)
															//{
															//	register int n;
															//	for (;;) {
															//		if (p->rfile != NULL) {
															//			/*
															//			 * 0 means EOF, so don't loop if we get 0.
															//			 */
															//			n = pcap_offline_read(p, cnt, callback, user);
															//		} else {
															//			/*
															//			 * XXX keep reading until we get something
															//			 * (or an merror occurs)
															//			 */
															//			do {
															//				n = p->read_op(p, cnt, callback, user);
															//			} while (n == 0);
															//		}
															//		if (n <= 0)
															//			return (n);
															//		if (!PACKET_COUNT_IS_UNLIMITED(cnt)) {
															//			cnt -= n;
															//			if (cnt <= 0)
															//				return (0);
															//		}
															//	}
															//}
		(void)fprintf(stderr, "%s: pcap_loop: %s\n",
		    program_name, pcap_geterr(pd));
		exit(1);
	}
	pcap_close(pd);									// what the declaration of function "pcap_close"? (refer to "pcap.c")
													//void
													//pcap_close(pcap_t *p)
													//{
													//	if (p->opt.source != NULL)
													//		free(p->opt.source);
													//	p->cleanup_op(p);
													//	free(p);
													//}
	exit(0);
}

/* routine is executed on exit */
void program_ending(int signo)
{
	struct pcap_stat stat;		// what is the exact structure of "pcap_stat"? (refer to "pcap\pcap.h")
								///*
								// * As returned by the pcap_stats()
								// */
								//struct pcap_stat {
								//	u_int ps_recv;		/* number of packets received */
								//	u_int ps_drop;		/* number of packets dropped */
								//	u_int ps_ifdrop;	/* drops by interface -- only supported on some platforms */
								//#ifdef WIN32
								//	u_int bs_capt;		/* number of packets that reach the application */
								//#endif /* WIN32 */
								//};
	if (pd != NULL && pcap_file(pd) == NULL) {		// what is the function of "pcap_file(pcap_t)"? (refer to "pcap.c")
													//FILE *
													//pcap_file(pcap_t *p)
													//{
													//	return (p->rfile);
													//}
		(void)fflush(stdout);	// what is the function of "fflush(..)"?
		putc('\n', stderr);
		if (pcap_stats(pd, &stat) < 0)				// what is the function of "pcap_stats(pcap_t, pcap_stat)"? (refer to "pcap.c")
													//int
													//pcap_stats(pcap_t *p, struct pcap_stat *ps)
													//{
													//	return (p->stats_op(p, ps));
													//}
			(void)fprintf(stderr, "pcap_stats: %s\n", pcap_geterr(pd));
		else {
			(void)fprintf(stderr, "%d packets received by filter\n", stat.ps_recv);
			(void)fprintf(stderr, "%d packets dropped by kernel\n", stat.ps_drop);
		}
	}
	exit(0);
}

/* Like default_print() but data need not be aligned */
void
default_print_unaligned(register const u_char *cp, register u_int length)
{
	register u_int i, s;
	register int nshorts;

	nshorts = (u_int) length / sizeof(u_short);
	i = 0;
	while (--nshorts >= 0) {
		if ((i++ % 8) == 0)
			(void)printf("\n\t\t\t");
		s = *cp++;								// what is the bit length of "u_char"? (8 bits)
		(void)printf(" %02x%02x", s, *cp++);	// 0xXXXX 0xXXXX ...
	}
	if (length & 1) {	// if the number of octets is an odd number...
		if ((i % 8) == 0)
			(void)printf("\n\t\t\t");
		(void)printf(" %02x", *cp);		// 0xXX
	}
}

/*
 * By default, print the packet out in hex.
 */
void
default_print(register const u_char *bp, register u_int length)
{
    Pack pack = Pack(bp, length);
    pack.viewInfo();
    pack.sendInfo(mSocket);
    /*
	register const u_short *sp;
    register const u_char *cp;

	register u_int i;
	register int nshorts;

	if ((long)bp & 1) {								// what is the result of (long)bp?
		default_print_unaligned(bp, length);
		return;
	}
	sp = (u_short *)bp;
    cp = (u_char *)bp;

	nshorts = (u_int) length / sizeof(u_short);		// what is the bit length of "u_short"? (16 bits)
	i = 0;
	while (--nshorts >= 0) {
		if ((i++ % 8) == 0)
			(void)printf("\n\t");
        (void)printf(" %c%c", *cp++, *cp++);
		//(void)printf(" %04x", ntohs(*sp++));		// what is the function of "ntohs()"? ( uint16_t ntohs(uint16_t netshort); <network to host short.> )
													// "Network Byte Order": we all convert our byte orderings to "big-endian" before sending them out. 
													// NOTE: Intel is a "little-endian" machine
	}
	if (length & 1) {	// if the number of octets is an odd number...
		if ((i % 8) == 0)
			(void)printf("\n\t");
		(void)printf(" %02x", *(u_char *)sp);		// what is the meaning of "%02x" or "%04x" (0xXX, 0xXXXX)
	}
     */
}


/*
insert your code in this routine

*/

void raw_print(u_char *user, const struct pcap_pkthdr *h, const u_char *p)
{
    printf("\tPacket #%d\n", cnt++);
    u_int length = h->len;
    u_int caplen = h->caplen;


    default_print(p, caplen);
    putchar('\n');
    putchar('\n');
}

void init_sender() {
    mSocket.create();
    mSocket.connect(2333);
}


