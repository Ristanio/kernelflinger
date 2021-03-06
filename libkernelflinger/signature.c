/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <openssl/x509.h>

#include "signature.h"
#include "lib.h"

/* This function must be defined by the module which support the
   compilation time selected signature format.  */
extern EFI_STATUS decode_boot_signature(const unsigned char *data, long size,
					struct boot_signature *bs);

void free_boot_signature(struct boot_signature *bs)
{
	if (!bs)
		return;

	FreePool(bs->signature);
	if (bs->id.parameters)
		FreePool(bs->id.parameters);
	if (bs->certificate)
		X509_free(bs->certificate);
	FreePool(bs);
}

struct boot_signature *get_boot_signature(const void *data, long size)
{
	EFI_STATUS ret;
	struct boot_signature *bs;

	bs = AllocatePool(sizeof(*bs));
	if (!bs)
		return NULL;

	ret = decode_boot_signature(data, size, bs);
	if (EFI_ERROR(ret)) {
		efi_perror(ret, L"Failed to decode signature");
		FreePool(bs);
		return NULL;
	}

	return bs;
}

/* vim: cindent:noexpandtab:softtabstop=8:shiftwidth=8:noshiftround
 */

