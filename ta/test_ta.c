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

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <test_ta.h>

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");

	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/*
	 * The DMSG() macro is non-standard, TEE Internal API doesn't
	 * specify any means to logging from a TA.
	 */
	IMSG("Hello HW_2! \n");

	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	IMSG("Goodbye! HW_2\n");
}

static TEE_Result inc_value(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	IMSG("Got value: %u from NW", params[0].value.a);
	params[0].value.a+=10;
	IMSG("Increase value to: %u", params[0].value.a);
	return TEE_SUCCESS;
}

static TEE_Result dec_value(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	IMSG("Got value: %u from NW", params[0].value.a);
	params[0].value.a--;
	IMSG("Decrease value to: %u", params[0].value.a);

	return TEE_SUCCESS;
}

static TEE_Result get_now_time(uint32_t param_types,
	TEE_Param params[4])
{
	
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	TEE_Time current_time;
	DMSG("get now time function has been called");
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
    TEE_GetREETime(&current_time);
	printf("current_time: %u %u \n",current_time.seconds,current_time.millis);
	params[0].value.a = current_time.seconds;
	params[0].value.b = current_time.millis;
	return TEE_SUCCESS;
}

static TEE_Result get_up_time(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	TEE_Time up_time;
	DMSG("get up time function has been called");
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
    TEE_GetSystemTime(&up_time);
	printf("up_time: %u %u \n",up_time.seconds,up_time.millis);
	params[0].value.a = up_time.seconds;
	params[0].value.b = up_time.millis;
	return TEE_SUCCESS;
}

static TEE_Result set_bit(uint32_t param_types,
	TEE_Param params[4]){
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	DMSG("set bit function has been called");
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
    params[0].value.a= params[0].value.a |( 1 << params[0].value.b);
    // OR x with the mask to set the y-th bit
	return TEE_SUCCESS;
}

static TEE_Result clear_bit(uint32_t param_types,
	TEE_Param params[4]){
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	DMSG("clear bit function has been called");
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
    params[0].value.a= params[0].value.a & ~(1 << params[0].value.b);
    // AND x with the mask to clear the y-th bit
	return TEE_SUCCESS;
}


static TEE_Result inverse_bit(uint32_t param_types,
	TEE_Param params[4]){
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	DMSG("inverse bit function has been called");
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
    params[0].value.a=params[0].value.a  ^ (1 << params[0].value.b);
	return TEE_SUCCESS;
}

static TEE_Result simulate_instruction(uint32_t param_types,
	TEE_Param params[4]){
    uint8_t opcode = (params[0].value.a >> 12) & 0xF;
    // Extract the first operand (bits 8-11)
    uint8_t operand1 = (params[0].value.a ) & 0x3F;
    // Extract the second operand (bits 4-7)
    uint8_t operand2 = (params[0].value.a >> 6) & 0x3F;
    

	printf("%d %d %d\n",operand1,operand2,opcode);
    // Perform operation based on opcode
    if (opcode == 0x1) { // ADD OPCODE = 0001
        params[0].value.a = operand1 + operand2;
    } else if (opcode == 0x5) { // AND OPCODE = 0101
        params[0].value.a = operand1 & operand2;
    } else {
        // NOT DEFINED OPCODE: DO NOTHING
		printf("++++%d %d\n",0x1,opcode);
		params[0].value.a=-1;
    }
	return TEE_SUCCESS;
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	switch (cmd_id) {
	case TA_TEST_CMD_INC_VALUE:
		return inc_value(param_types, params);
	case TA_TEST_CMD_DEC_VALUE:
		return dec_value(param_types, params);
	case TA_TEST_CMD_RETTIME_VALUE:
		return get_now_time(param_types,params);
	case TA_TEST_CMD_RETUPTIME_VALUE:
		return get_up_time(param_types,params);
	case TA_TEST_CMD_SET_BIT:
		return set_bit(param_types,params);
	case TA_TEST_CMD_CLEAR_BIT:
		return clear_bit(param_types,params);
	case TA_TEST_CMD_INVERSE_BIT:
		return inverse_bit(param_types,params);
	case TA_TEST_CMD_SIMULATE_INSTRUCTION:
		return simulate_instruction(param_types,params);

	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
