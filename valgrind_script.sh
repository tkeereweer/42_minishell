#!/bin/bash
valgrind --show-leak-kinds=all --track-fds=yes --track-origins=yes --leak-check=full --suppressions=supp.supp ./minishell