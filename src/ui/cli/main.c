/*    
 *  This file is part of BitFiend. 
 *  Copyright (C) 2017 Eduard Permyakov 
 *
 *  BitFiend is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  BitFiend is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <libbf/bitfiend.h>
#include "printclr.h"
#include "commands.h"

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define CLI_VER_MAJOR 0
#define CLI_VER_MINOR 1
#define CLI_VER_PATCH 0

static void print_welcome(void)
{
    printf("    ____  _ __  _______                __\n");
    printf("   / __ )(_) /_/ ____(_)__  ____  ____/ /\n");
    printf("  / __  / / __/ /_  / / _ \\/ __ \\/ __  / \n");
    printf(" / /_/ / / /_/ __/ / /  __/ / / / /_/ /  \n");
    printf("/_____/_/\\__/_/   /_/\\___/_/ /_/\\__,_/   ");
    printf("libbf %d.%d.%d cli %d.%d.%d\n\n", LIBBF_VER_MAJOR, LIBBF_VER_MINOR, LIBBF_VER_PATCH,
        CLI_VER_MAJOR, CLI_VER_MINOR, CLI_VER_PATCH);
    printf("Copyright (c) 2017 Eduard Permyakov\n");
    printf("This is free software: you are free to change and redistribute it.\n");
    printf("Type \"help\" for a list of commands or \"exit\" to quit.\n");
}

static void print_prompt(void)
{
    printclr(FG_YELLOW, "BitFiend> ");
}

static void next_line(char *out, size_t n)
{
    fgets(out, n, stdin);
    out[n-1] = '\0';
    /* If input was too long, consume the rest of stdin buffer */
    if(out[strlen(out)-1] != '\n'){
        char c;
        while (((c = getchar()) != '\n') && (c != EOF));
    }
}

static bool is_empty_line(const char *str) 
{
    while (*str) {
        if (!isspace(*str))
            return false;
        str++;
    }
    return true;
}

int main(int argc, char **argv)
{
    if(argc != 1){
        printf("Usage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    print_welcome();

    signal(SIGINT, SIG_IGN); //nicer handling of SIGINT eventually
    signal(SIGPIPE, SIG_IGN);

    if(bitfiend_init("./bitfiend.log") != BITFIEND_SUCCESS){
        fprintf(stderr, "Failed initializing libbf. Check the logs!\n");
        exit(EXIT_FAILURE);
    }

    while(true) {
        print_prompt();

        char line[256];
        next_line(line, sizeof(line));

        if(is_empty_line(line))
            continue;

        if(command_parse_and_exec(line) == CMD_FAIL_EXEC)
            printf("%s is not a valid command. See \"help\".\n", line);
    }

    /* Client will be shutdown by exit command */
    assert(0);
}

