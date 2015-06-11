/* Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */


#ifndef _MYSQLX_ERRORS_H_
#define _MYSQLX_ERRORS_H_


#define ER_X_INVALID_ARGUMENT     5012
#define ER_X_MISSING_ARGUMENT     5013
#define ER_X_BAD_INSERT_DATA      5014

#define ER_X_CMD_NUM_ARGUMENTS    5015
#define ER_X_CMD_ARGUMENT_TYPE    5016
#define ER_X_CMD_ARGUMENT_VALUE   5017

#define ER_X_BAD_STATEMENT_ID     5110
#define ER_X_BAD_CURSOR_ID        5111

#define ER_X_BAD_SCHEMA           5112
#define ER_X_BAD_TABLE            5113
#define ER_X_BAD_PROJECTION       5114

#define ER_X_DOC_ID_MISSING       5115
#define ER_X_DOC_ID_DUPLICATE     5116
#define ER_X_DOC_REQUIRED_FIELD_MISSING 5117


#define ER_X_PROJ_BAD_KEY_NAME    5120
#define ER_X_BAD_DOC_PATH         5121

#define ER_X_CURSOR_EXISTS        5122

#define ER_X_EXPR_BAD_OPERATOR    5150
#define ER_X_EXPR_BAD_NUM_ARGS    5151
#define ER_X_EXPR_MISSING_ARG     5152
#define ER_X_EXPR_BAD_TYPE_VALUE  5153
#define ER_X_EXPR_BAD_VALUE       5154

#define ER_X_INVALID_COLLECTION   5156

#define ER_X_INVALID_ADMIN_COMMAND 5157

#endif
