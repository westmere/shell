/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    shell.h
 * @brief   Simple CLI shell header.
 *
 * @addtogroup SHELL
 * @{
 */

#ifndef SHELL_H_
#define SHELL_H_

/**
 * @brief   Shell maximum input line length.
 */
#if !defined(SHELL_LINE_BUFFER_LENGTH) || defined(__DOXYGEN__)
#define SHELL_LINE_BUFFER_LENGTH       64
#endif

/**
 * @brief   Shell maximum arguments per command.
 */
#if !defined(SHELL_ARGUMENTS) || defined(__DOXYGEN__)
#define SHELL_ARGUMENTS         4
#endif

typedef struct shell shell_t;
typedef struct shell_command shell_commant_t;

/**
 * @brief   Shell descriptor type.
 */
typedef struct {
  const shell_commant_t    *commands;       /**< @brief Shell extra commands
                                                 table.                     */
} shell_config_t;

/**
 * @brief   Command handler function type.
 */
typedef void (*shellexe_t)(void *arg, int argc, char *argv[]);

/**
 * @brief   Custom command entry type.
 */
struct shell_command{
  const char *name;   /**< @brief Command name.       */
  shellexe_t exe;     /**< @brief Command function.   */
};

/**
 * @brief   Shell descriptor type.
 */
typedef struct {
	int (*putchar)(int ch);
	int (*getchar)(void);
//	int (*printf)(const char *fmt,...);
}shell_stdio_t;

/**
 * @brief   Shell descriptor type.
 */
struct shell{
	const shell_config_t *config;
	void *arg;
	char cmd[8];
};

enum SHELL_RESULT{
	SHELL_RESULT_EXIT=-4,
	SHELL_RESULT_TOO_MANY_ARGUMENTS=-3,
	SHELL_RESULT_COMMAND_NOT_FOUND=-2,
	SHELL_RESULT_NO_COMMAND=-1
};

#ifdef __cplusplus
extern "C" {
#endif
  void shellInit(shell_t *shell, void *arg, const shell_config_t *config);
  void shellExit(shell_t *shell);
  enum SHELL_RESULT shellProcessLine(shell_t *shell, char *line);
  int shellGetLine(shell_t *shell, const shell_stdio_t *stdio, char *line, unsigned int size);
#ifdef __cplusplus
}
#endif

#endif /* _SHELL_H_ */

/** @} */
