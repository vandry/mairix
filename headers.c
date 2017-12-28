#include <stddef.h>
#include <string.h>

#include "headers.h"

/**
 * Table of single-character abbreviations for common headers. There are a lot
 * of other headers that are very common that could be added to this list, but
 * they are specific to certain email service providers and / or mail servers.
 */
char *common_prefix_table[256] = {
    [1] = "accept-language",
    [2] = "arc-authentication-results",
    [3] = "arc-message-signature",
    [4] = "arc-seal",
    [5] = "authentication-results",
    [6] = "content-disposition",
    [7] = "content-id",
    [8] = "content-language",
    [9] = "content-length",
   [10] = "content-transfer-encoding",
   [11] = "content-type",
   [12] = "delivered-to",
   [13] = "dkim-signature",
   [14] = "in-reply-to",
   [15] = "list-archive",
   [16] = "list-id",
   [17] = "list-post",
   [18] = "list-unsubscribe",
   [19] = "message-id",
   [20] = "mime-version",
   [21] = "precedence",
   [22] = "received-spf",
   [23] = "received",
   [24] = "references",
   [25] = "reply-to",
   [26] = "return-path",
   [27] = "sender",
   [28] = "thread-index",
   [29] = "thread-topic",
   [30] = "user-agent",
   [31] = "x-auto-response-suppress",
   [32] = "x-feedback-id",

  /* ASCII values 33 through 126 are valid RFC 822 field name characters. */

  [126] = "x-mailer",
  [127] = "x-originating-ip",
  [128] = "x-originatororg",
  [129] = "x-priority",
  [130] = "x-received",
  [131] = "x-sasl-enc",

  [255] = NULL, /* 0xff is used as the token terminator in the database. */
};

/**
 * If a token has an abbreviated prefix, return the human-readable version.
 * Otherwise, NULL is returned.
 */
inline char *resolve_prefix(char *token_in_database)
{
  return common_prefix_table[(unsigned char) *token_in_database];
}

/**
 * Return a token stripped of its prefix. The `accepted_prefixes` is a NULL
 * terminated array of strings. If the `accepted_prefixes` is defined, a NULL
 * is returned if the token prefix is not present in that list.
 */
inline char *token_without_prefix(char *token, char **accepted_prefixes)
{
  char *prefix;
  char *colon;
  char **entry;

  if (!accepted_prefixes) return strchr(token, ':') + 1;

  if ((prefix = common_prefix_table[(unsigned char) *token])) {
    colon = token + 2;
    for (entry = accepted_prefixes; *entry; entry++)
      if (!strcasecmp(*entry, prefix))
        return token + 2;
  } else {
    colon = strchr(token, ':');
    for (entry = accepted_prefixes; *entry; entry++)
      if (!strncasecmp(*entry, token, colon - token) && (*entry)[colon - token] == '\0')
        return colon + 1;
  }

  return NULL;
}

/**
 * Attempt to compress a token prefix and returned the resulting value.
 */
inline char *compress_prefix(char *prefix)
{
  static char result[3];

  for (int i = 1; i < 256; i++) {
    if (common_prefix_table[i] && !strcasecmp(prefix, common_prefix_table[i])) {
      result[0] = '\0' + i;
      result[1] = '\0';
      return result;
    }
  }

  return prefix;
}

/**
 * Indicates whether or not a table uses prefixes. This exists mostly for
 * readability since only the minor headers table uses prefixes.
 */
inline int table_tokens_have_prefixes(struct read_db *db, struct toktable_db *tt)
{
  return tt == &db->minor_headers;
}
