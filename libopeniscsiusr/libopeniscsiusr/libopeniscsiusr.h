/*
 * Copyright (C) 2017 Red Hat, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Gris Ge <fge@redhat.com>
 */
#ifndef _LIB_OPEN_ISCSI_USR_H_
#define _LIB_OPEN_ISCSI_USR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdarg.h>

#include "libopeniscsiusr_common.h"
#include "libopeniscsiusr_session.h"
#include "libopeniscsiusr_iface.h"

/**
 * iscsi_log_priority_str() - Convert log priority to string.
 *
 * Convert log priority to string (const char *).
 *
 * @priority:
 *	int. Log priority.
 *
 * Return:
 *	const char *. Please don't free returned pointer. Valid string are:
 *
 *	* "ERROR" for LIBISCSI_LOG_PRIORITY_ERROR
 *
 *	* "WARN"  for LIBISCSI_LOG_PRIORITY_WARNING
 *
 *	* "INFO"  for LIBISCSI_LOG_PRIORITY_INFO
 *
 *	* "DEBUG" for LIBISCSI_LOG_PRIORITY_DEBUG
 *
 *	* "Invalid argument" for invalid log priority.
 */
__DLL_EXPORT const char *iscsi_log_priority_str(int priority);

/**
 * iscsi_strerror() - Convert error code to string.
 *
 * Convert error code (int) to string (const char *):
 *
 *	* LIBISCSI_OK -- "OK"
 *
 *	* LIBISCSI_ERR_BUG -- "BUG of libopeniscsiusr library"
 *
 *	* LIBISCSI_ERR_SESS_NOT_FOUND - "Specified iSCSI session not found"
 *
 *	* LIBISCSI_ERR_ACCESS - "Permission deny"
 *
 *	* LIBISCSI_ERR_NOMEM - "Out of memory"
 *
 *	* LIBISCSI_ERR_SYSFS_LOOKUP - "Could not lookup object in sysfs"
 *
 *	* Other invalid error number -- "Invalid argument"
 *
 * @rc:
 *	int. Return code by libiscsiur functions. When provided error code is
 *	not a valid error code, return "Invalid argument".
 *
 * Return:
 *	const char *. The meaning of provided error code. Don't free returned
 *	pointer.
 */
__DLL_EXPORT const char *iscsi_strerror(int rc);

/**
 * iscsi_context_new() - Create struct iscsi_context.
 *
 * The default logging level (LIBISCSI_LOG_PRIORITY_DEFAULT) is
 * LIBISCSI_LOG_PRIORITY_WARNING which means only warning and error message will
 * be forward to log handler function.  The default log handler function will
 * print log message to STDERR, to change so, please use
 * iscsi_context_log_func_set() to set your own log handler, check manpage
 * libopeniscsiusr.h(3) for detail.
 *
 * Return:
 *	Pointer of 'struct iscsi_context'. Should be freed by
 *	iscsi_context_free().
 */
__DLL_EXPORT struct iscsi_context *iscsi_context_new(void);

/**
 * iscsi_context_free() - Release the memory of struct iscsi_context.
 *
 * Release the memory of struct iscsi_context, but the userdata memory defined
 * via iscsi_context_userdata_set() will not be touched.
 *
 * @ctx:
 *	Pointer of 'struct iscsi_context'.
 * Return:
 *	void
 */
__DLL_EXPORT void iscsi_context_free(struct iscsi_context *ctx);

/**
 * iscsi_context_log_priority_set() - Set log priority.
 *
 *
 * When library generates log message, only equal or more important(less value)
 * message will be forwarded to log handler function. Valid log priority values
 * are:
 *
 *	* LIBISCSI_LOG_PRIORITY_ERROR -- 3
 *
 *	* LIBISCSI_LOG_PRIORITY_WARNING -- 4
 *
 *	* LIBISCSI_LOG_PRIORITY_INFO -- 6
 *
 *	* LIBISCSI_LOG_PRIORITY_DEBUG -- 7
 *
 * @ctx:
 *	Pointer of 'struct iscsi_context'.
 *	If this pointer is NULL, your program will be terminated by assert.
 *
 * @priority:
 *	int, log priority.
 *
 * Return:
 *	void
 */
__DLL_EXPORT void iscsi_context_log_priority_set(struct iscsi_context *ctx,
						 int priority);

/**
 * iscsi_context_log_priority_get() - Get log priority.
 *
 * Retrieve current log priority. Valid log priority values are:
 *
 *	* LIBISCSI_LOG_PRIORITY_ERROR -- 3
 *
 *	* LIBISCSI_LOG_PRIORITY_WARNING -- 4
 *
 *	* LIBISCSI_LOG_PRIORITY_INFO -- 5
 *
 *	* LIBISCSI_LOG_PRIORITY_DEBUG -- 7
 *
 * @ctx:
 *	Pointer of 'struct iscsi_context'.
 *	If this pointer is NULL, your program will be terminated by assert.
 *
 * Return:
 *	int, log priority.
 */
__DLL_EXPORT int iscsi_context_log_priority_get(struct iscsi_context *ctx);

/**
 * iscsi_context_log_func_set() - Set log handler function.
 *
 * Set custom log handler. The log handler will be invoked when log message
 * is equal or more important(less value) than log priority setting.
 * Please check manpage libopeniscsiusr.h(3) for detail usage.
 *
 * @ctx:
 *	Pointer of 'struct iscsi_context'.
 *	If this pointer is NULL, your program will be terminated by assert.
 * @log_func:
 *	Pointer of log handler function.
 *
 * Return:
 *	void
 */
__DLL_EXPORT void iscsi_context_log_func_set
	(struct iscsi_context *ctx,
	 void (*log_func) (struct iscsi_context *ctx, int priority,
			   const char *file, int line, const char *func_name,
			   const char *format, va_list args));

/**
 * iscsi_context_userdata_set() - Set user data pointer.
 *
 * Store user data pointer into 'struct iscsi_context'.
 *
 * @ctx:
 *	Pointer of 'struct iscsi_context'.
 *	If this pointer is NULL, your program will be terminated by assert.
 * @userdata:
 *	Pointer of user defined data.
 *
 * Return:
 *	void
 */
__DLL_EXPORT void iscsi_context_userdata_set(struct iscsi_context *ctx,
					     void *userdata);

/**
 * iscsi_context_userdata_get() - Get user data pointer.
 *
 * Retrieve user data pointer from 'struct iscsi_context'.
 *
 * @ctx:
 *	Pointer of 'struct iscsi_context'.
 *	If this pointer is NULL, your program will be terminated by assert.
 *
 * Return:
 *	void *. Pointer of user defined data.
 */
__DLL_EXPORT void *iscsi_context_userdata_get(struct iscsi_context *ctx);

/**
 * iscsi_sessions_get() - Retrieve all iSCSI sessions.
 *
 * Retrieves all iSCSI sessions. For the properties of 'struct iscsi_session',
 * please refer to the functions defined in 'libopeniscsiusr_session.h' file.
 *
 * @ctx:
 *	Pointer of 'struct iscsi_context'.
 *	If this pointer is NULL, your program will be terminated by assert.
 * @ses:
 *	Output pointer of 'struct iscsi_session' pointer array. Its memory
 *	should be freed by iscsi_sessions_free().
 *	If this pointer is NULL, your program will be terminated by assert.
 * @se_count:
 *	Output pointer of uint32_t. Will store the size of
 *	'struct iscsi_session' pointer array.
 *
 * Return:
 *	int. Valid error codes are:
 *
 *	* LIBISCSI_OK
 *
 *	* LIBISCSI_ERR_BUG
 *
 *	* LIBISCSI_ERR_NOMEM
 *
 *	* LIBISCSI_ERR_ACCESS
 *
 *	* LIBISCSI_ERR_SYSFS_LOOKUP
 *
 *	Error number could be converted to string by iscsi_strerror().
 */
__DLL_EXPORT int iscsi_sessions_get(struct iscsi_context *ctx,
				    struct iscsi_session ***ses,
				    uint32_t *se_count);

/**
 * iscsi_sessions_free() - Free the memory of 'struct iscsi_session' pointer
 * array
 *
 * Free the memory of 'iscsi_session' pointer array generated by
 * 'iscsi_sessions_get()'.
 * If provided 'ses' pointer is NULL or 'session_count' is 0, do nothing.
 *
 * @ses:
 *	Pointer of 'struct iscsi_session' pointer array.
 * @session_count:
 *	uint32_t, the size of 'struct iscsi_session' pointer array.
 *
 * Return:
 *	void
 */
__DLL_EXPORT void iscsi_sessions_free(struct iscsi_session **ses,
				      uint32_t session_count);

/**
 * iscsi_session_get() - Retrieve specified iSCSI sessions.
 *
 * Retrieves specified iSCSI sessions. For the properties of
 * 'struct iscsi_session', please refer to the functions defined in
 * 'libopeniscsiusr_session.h' file.
 *
 * @ctx:
 *	Pointer of 'struct iscsi_context'.
 *	If this pointer is NULL, your program will be terminated by assert.
 * @sid:
 *	uint32_t, iSCSI session ID.
 * @se:
 *	Output pointer of 'struct iscsi_session' pointer. Its memory
 *	should be freed by iscsi_session_free().
 *	If this pointer is NULL, your program will be terminated by assert.
 *	If specified iSCSI session does not exists, this pointer will be set to
 *	NULL with LIBISCSI_OK returned.
 *
 * Return:
 *	int. Valid error codes are:
 *
 *	* LIBISCSI_OK
 *
 *	* LIBISCSI_ERR_BUG
 *
 *	* LIBISCSI_ERR_NOMEM
 *
 *	* LIBISCSI_ERR_ACCESS
 *
 *	* LIBISCSI_ERR_SYSFS_LOOKUP
 *
 *	* LIBISCSI_ERR_SESS_NOT_FOUND
 *
 *	Error number could be converted to string by iscsi_strerror().
 */
__DLL_EXPORT int iscsi_session_get(struct iscsi_context *ctx, uint32_t sid,
				   struct iscsi_session **se);

/**
 * iscsi_session_free() - Free the memory of 'struct iscsi_session'.
 *
 * Free the memory of 'iscsi_session' pointer generated by
 * 'iscsi_sessions_get()'.
 * If provided 'se' pointer is NULL, do nothing.
 *
 * @se:
 *	Pointer of 'struct iscsi_session' pointer.
 *
 * Return:
 *	void
 */
__DLL_EXPORT void iscsi_session_free(struct iscsi_session *se);


#ifdef __cplusplus
} /* End of extern "C" */
#endif

#endif /* End of _LIB_OPEN_ISCSI_USR_H_ */
