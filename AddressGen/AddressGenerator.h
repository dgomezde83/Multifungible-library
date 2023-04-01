#ifndef ADRESSGENERATOR_H
#define ADRESSGENERATOR_H

#define ADDR_TYPE_EGLD -3
#define PRIV_TYPE_EGLD -3

enum vg_format {
	VCF_PUBKEY,
	VCF_SCRIPT,
	VCF_CONTRACT, // VCF_CONTRACT only valid for ETH
	VCF_P2WPKH,
	VCF_P2TR
};

typedef struct ApplicationLevelContext ALC;

#define addressgenerator_max_threads 1024

/* Application-level context. parameters and global pattern store */
struct ApplicationLevelContext {
    int vc_addrtype;
    int vc_privtype;
    const char *vc_result_file;
    int vc_numpairs;
    char *pattern;
    int match_location; // 0: any, 1: begin, 2: end
    int vc_verbose;
    enum vg_format vc_format;
    int vc_halt;
    int vc_found_num;
    unsigned long vc_start_time;
    int vc_thread_num;
    unsigned long long vc_check_count[addressgenerator_max_threads];
    char * cpp_pub_address;
    char * cpp_priv_address;
    const char * key_password;
    const char * keyFilePath;
};

int start_threads_address_generation(ALC *);

#endif //ADRESSGENERATOR_H
