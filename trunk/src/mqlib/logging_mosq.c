/*
Copyright (c) 2009,2010, Roger Light <roger@atchoo.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of mosquitto nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <mosquitto_internal.h>
#include <mosquitto.h>

int mosquitto_log_init(struct mosquitto *mosq, int priorities, int destinations)
{
	assert(mosq);

	mosq->log_priorities = priorities;
	mosq->log_destinations = destinations;

	return MOSQ_ERR_SUCCESS;
}

int _mosquitto_log_printf(struct mosquitto *mosq, int priority, const char *fmt, ...)
{
	va_list va;
	char s[500];

	assert(mosq);

	if((mosq->log_priorities & priority) && mosq->log_destinations != MOSQ_LOG_NONE){
		va_start(va, fmt);
		vsnprintf(s, 500, fmt, va);
		va_end(va);

		if(mosq->log_destinations & MOSQ_LOG_STDOUT){
			fprintf(stdout, "%s\n", s);
			fflush(stdout);
		}
		if(mosq->log_destinations & MOSQ_LOG_STDERR){
			fprintf(stderr, "%s\n", s);
			fflush(stderr);
		}
	}

	return MOSQ_ERR_SUCCESS;
}
