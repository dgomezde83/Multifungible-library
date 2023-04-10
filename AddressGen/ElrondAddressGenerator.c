/*
 * Vanitygen, vanity bitcoin address generator
 * Copyright (C) 2011 <samr7@cs.washington.edu>
 *
 * Vanitygen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Vanitygen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Vanitygen.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

#include "util.h"
#include "AddressGenerator.h"
#include "ElrondAddressGenerator.h"


int GRSFlag = 0;

int getElrondAddress(char * cpp_priv_address, const char * p_password, const char * p_keyFilePath)
{
	int addrtype = 0;
	int privtype = 128;
	int verbose = 1;
	int numpairs = 0;
	const char *result_file = NULL;
	int nthreads = 0;

	addrtype = ADDR_TYPE_EGLD;
    privtype = PRIV_TYPE_EGLD;

#if OPENSSL_VERSION_NUMBER < 0x10000000L
	/* Complain about older versions of OpenSSL */
	if (verbose > 0) {
		fprintf(stderr,
			"WARNING: Built with " OPENSSL_VERSION_TEXT "\n"
			"WARNING: Use OpenSSL 1.0.0d+ for best performance\n");
	}
#endif


//#if OPENSSL_VERSION_NUMBER < 0x30000000L
		//fprintf(stderr, "OpenSSL 3.0 (or higher) is required for Cosmos address\n");
		//return 1;
//#else

	ALC * t_alc = NULL;
	t_alc = (ALC *) malloc(sizeof(*t_alc));
	t_alc->vc_verbose = verbose;
	t_alc->vc_addrtype = addrtype;
	t_alc->vc_privtype = privtype;
	t_alc->vc_result_file = result_file;
	t_alc->vc_numpairs = numpairs;
	t_alc->cpp_priv_address = cpp_priv_address;
    t_alc->key_password = p_password;
    t_alc->keyFilePath = p_keyFilePath;
    t_alc->vc_halt = 0;
    t_alc->vc_found_num = 0;

	if (t_alc->vc_numpairs == 0) {
		t_alc->vc_numpairs = 1;
	}
	t_alc->pattern = "erd";
	t_alc->match_location = 1; // By default, match begin location


	if (nthreads <= 0) {
		/* Determine the number of threads */
		nthreads = count_processors();
		if (nthreads <= 0) {
			fprintf(stderr, "ERROR: could not determine processor count\n");
			nthreads = 1;
		}

		if (nthreads > addressgenerator_max_threads) {
			fprintf(stderr, "WARNING: too many threads\n");
			nthreads = addressgenerator_max_threads;
		}
	}
	t_alc->vc_thread_num = nthreads;
	t_alc->vc_start_time = (unsigned long)time(NULL);

	if (!start_threads_address_generation(t_alc))
		return 1;

	return 0;
//#endif
}
