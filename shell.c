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
 * @file    shell.c
 * @brief   Simple CLI shell code.
 *
 * @addtogroup SHELL
 * @{
 */

#include <stdlib.h>
#include <string.h>
#include "shell.h"

static char *_strtok(char *str, const char *delim, char **saveptr) {
  char *token;
  if (str)
    *saveptr = str;
  token = *saveptr;

  if (!token)
    return NULL;

  token += strspn(token, delim);
  *saveptr = strpbrk(token, delim);
  if (*saveptr)
    *(*saveptr)++ = '\0';

  return *token ? token : NULL;
}

static int shell_puts(const shell_stdio_t *stdio, const char *s)
{
	while(*s)
		stdio->putchar(*s++);
	return 0;
}

static void usage(const shell_stdio_t *stdio, char *p) {

  shell_puts(stdio, "Usage: ");
  shell_puts(stdio, p);
  shell_puts(stdio, "\r\n");
}

static void list_commands(shell_t *shell, const shell_stdio_t *stdio) {
	const shell_commant_t *scp=shell->config->commands;

	while (scp->name != NULL)
	{
		shell_puts(stdio, scp->name);
		stdio->putchar(' ');
		scp++;
	}
}

/**
 * @brief   Shell manager initialization.
 *
 * @api
 */
void shellInit(shell_t *shell, void *arg, const shell_config_t *config)
{
	shell->arg = arg;
	shell->config = config;
}

enum SHELL_RESULT shellProcessLine(shell_t *shell, char *line)
{
	char *lp, *cmd, *tokp;
	char *args[SHELL_ARGUMENTS + 1];
	int n;
	const shell_commant_t *scp=shell->config->commands;

	lp = strtok_r(line, " \t", &tokp);
	cmd = lp;
	n = 0;
	while ((lp = strtok_r(NULL, " \t", &tokp)) != NULL )
	{
		if (n >= SHELL_ARGUMENTS)
		{
			return SHELL_RESULT_TOO_MANY_ARGUMENTS;
		}
		args[n++] = lp;
	}
	args[n] = NULL;
	if (cmd != NULL)
	{
		strlcpy(shell->cmd, cmd, 8);
		if (strcasecmp(cmd, "exit") == 0)
		{
			if (!n)
			{
				return SHELL_RESULT_EXIT;
			}
		}
		else
		{
			if (scp)
			{
				int i;

				for(i = 0;scp[i].name != NULL;i++)
				{
					if (strcasecmp(scp[i].name, cmd) == 0)
					{
						scp[i].exe(shell->arg, n, args);
						return i;
					}
				}
				return SHELL_RESULT_COMMAND_NOT_FOUND;
			}
		}
	}
	return SHELL_RESULT_NO_COMMAND;
}

/**
 * @brief   Terminates the shell.
 * @note    Must be invoked from the command handlers.
 * @note    Does not return.
 *
 * @param[in] msg       shell exit code
 *
 * @api
 */
void shellExit(shell_t *shell) {

}

/**
 * @brief   Reads a whole line from the input channel.
 *
 * @param[in] shell       pointer to a @p BaseSequentialStream object
 * @param[in] line      pointer to the line buffer
 * @param[in] size      buffer maximum length
 * @return              The operation status.
 * @retval true         the channel was reset or CTRL-D pressed.
 * @retval false        operation successful.
 *
 * @api
 */
int shellGetLine(shell_t *shell, const shell_stdio_t *stdio, char *line, unsigned int size)
{
	char *p = line;

	for(;;)
	{
		int c;

		c = stdio->getchar();
		if (0 > c)
			return -1;
		if (c == 4)
		{
			stdio->putchar('^');
			stdio->putchar('D');
			return -1;
		}
		if ((c == 8) || (c == 127))
		{
			if (p != line)
			{
				stdio->putchar(0x08);
				stdio->putchar(0x20);
				stdio->putchar(0x08);
				p--;
			}
			continue;
		}
		if (c == '\r')
		{
			shell_puts(stdio, "\r\n");
			*p = 0;
			return 0;
		}
		if (c < 0x20)
			continue;
		if (p < line + size - 1) {
			stdio->putchar(c);
			*p++ = (char) c;
		}
	}
}

/** @} */
