#ifndef HEADERS_H
#define HEADERS_H

#include "mairix.h"
#include "reader.h"

extern char *common_prefix_table[256];

char *compress_prefix(char *prefix);
char *resolve_prefix(char *token_in_database);
char *token_without_prefix(char *token, char **accepted_prefixes);
int table_tokens_have_prefixes(struct read_db *db, struct toktable_db *tt);

#endif
