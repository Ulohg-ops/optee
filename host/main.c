/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <err.h>
#include <stdio.h>
#include <string.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* For the UUID (found in the TA's h-file(s)) */
#include <test_ta.h>
#include <time.h>

// void combine_seconds_and_millis(uint32_t second,uint32_t millis)
// {
//     uint64_t timestamp_ms = (uint64_t)second * 1000 + millis;

//     printf("System time：%lu\n", timestamp_ms);
// }
void convertUnixTimestampToUptime(uint32_t unixTimestamp) {
	int hours = unixTimestamp / 3600;
    int minutes = (unixTimestamp % 3600) / 60;
    int seconds = unixTimestamp % 60;
	printf("System uptime: %d hours, %d minutes, %d seconds\n", hours, minutes, seconds);
}

void format_timestamp(uint32_t timestamp_second)
{  
    time_t seconds = timestamp_second;
    // uint16_t millis = timestamp_ms % 1000;

    struct tm local_time;
    localtime_r(&seconds, &local_time);

    char datetime_str[64];
    strftime(datetime_str, sizeof(datetime_str), "%Y-%m-%d %H:%M:%S", &local_time);

    printf("Current time :%s UTC\n", datetime_str);
}


int main(void)
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_TEST_UUID;
	uint32_t err_origin;

	/* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	/*
	 * Open a session to the "hello world" TA, the TA will print "hello
	 * world!" in the log when the session is created.
	 */
	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

	/*
	 * Execute a function in the TA by invoking it, in this case
	 * we're incrementing a number.
	 *
	 * The value of command ID part and how the parameters are
	 * interpreted is part of the interface provided by the TA.
	 */

	/* Clear the TEEC_Operation struct */
	memset(&op, 0, sizeof(op));

	/*
	 * Prepare the argument. Pass a value in the first parameter,
	 * the remaining three parameters are unused.
	 */
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
	// o
	

	/*
	 * TA_TEST_CMD_INC_VALUE is the actual function in the TA to be
	 * called.
	 */
	 
	// printf("Invoking TA to increment %d\n", op.params[0].value.a);
	// res = TEEC_InvokeCommand(&sess, TA_TEST_CMD_INC_VALUE, &op,
	// 			 &err_origin);
	// if (res != TEEC_SUCCESS)
	// 	errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
	// 		res, err_origin);
	// printf("TA incremented value to %d\n", op.params[0].value.a);
	

	res = TEEC_InvokeCommand(&sess, TA_TEST_CMD_RETTIME_VALUE, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
	// printf("get second from TA :%d\n",op.params[0].value.a);
	// printf("get millies from TA :%d\n",op.params[0].value.b);
	format_timestamp(op.params[0].value.a);
	
	res = TEEC_InvokeCommand(&sess, TA_TEST_CMD_RETUPTIME_VALUE, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
	convertUnixTimestampToUptime(op.params[0].value.a);

	op.params[0].value.a = 2454;
	op.params[0].value.b=3;
	printf("X:%d Y:%d\n",op.params[0].value.a,op.params[0].value.b);
	res = TEEC_InvokeCommand(&sess, TA_TEST_CMD_SET_BIT, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
	printf("invoked set bit new X:%d \n",op.params[0].value.a);

	op.params[0].value.a = 2454;
	op.params[0].value.b=3;
	printf("X:%d Y:%d\n",op.params[0].value.a,op.params[0].value.b);
	res = TEEC_InvokeCommand(&sess, TA_TEST_CMD_CLEAR_BIT, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
	printf("invoked clear bit new X:%d \n",op.params[0].value.a);

	op.params[0].value.a = 2454;
	op.params[0].value.b=3;
	printf("X:%d Y:%d\n",op.params[0].value.a,op.params[0].value.b);
	res = TEEC_InvokeCommand(&sess, TA_TEST_CMD_INVERSE_BIT, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
	printf("invoked inverse bit new X:%d \n",op.params[0].value.a);

	op.params[0].value.a = 4291;
	printf("machine code:%d\n",op.params[0].value.a);
	res = TEEC_InvokeCommand(&sess, TA_TEST_CMD_SIMULATE_INSTRUCTION, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
	printf("Result:%d \n",op.params[0].value.a);

	/*
	 * We're done with the TA, close the session and
	 * destroy the context.
	 *
	 * The TA will print "Goodbye!" in the log when the
	 * session is closed.
	 */


	TEEC_CloseSession(&sess);

	TEEC_FinalizeContext(&ctx);

	return 0;
}


