#include "l4arg.h"
#include "l4array.h"

#include <stdlib.h>
#include <string.h>

LbsStrv* lbs_arg_parse (const char* str, const char* delim,
	const char* esc, const LbsArgQuote* q, LbsArray** detail_ptr) {

	LbsStrv* strv = lbs_strv_new ();
	if (strv == NULL) {
		return NULL;
	}
	/* goto label => free_strv */

	// string length cache
	int qlen; // user-supplied quoting string table length
	for (qlen = 0; q[qlen].left != NULL && q[qlen].right != NULL; qlen++);

	// qlen will not be too long, so we can use VLA
	int qlen_p = qlen <= 0 ? 1 : qlen;

#if __STDC_NO_VLA__
	int* qllen = malloc (sizeof (int) * qlen_p);
	if (qllen == NULL) {
		lbs_strv_unref (strv);
		return NULL;
	}
	int* qrlen = malloc (sizeof (int) * qlen_p);
	if (qrlen == NULL) {
		lbs_strv_unref (strv);
		return NULL;
	}
#else
	int qllen[qlen_p]; // left quoting string length
	int qrlen[qlen_p]; // right quoting string length
#endif

	for (int i = 0; i < qlen; i++) {
		// empty strings are not allowed
		qllen[i] = strlen (q[i].left);
		if (qllen[i] <= 0) {
			goto free_strv;
		}
		qrlen[i] = strlen (q[i].right);
		if (qrlen[i] <= 0) {
			goto free_strv;
		}
	}

	LbsArray* detail;
	if (detail_ptr != NULL) {
		detail = lbs_array_new (sizeof (int));
		if (detail == NULL) {
			goto free_strv;
		}
		if (lbs_array_push_back (detail, &(int){-1}) < 0) {
			goto free_detail;
		}
	} else {
		detail = NULL;
	}
	/* goto label => free_detail */

	bool is_delimed = true;
	bool is_escaped = false;
	bool is_quoted = false;
	bool ignore_esc = false;
	int stri = 0; // strv index
	int qi; // quoting string index currently used

	const char* p = str;
	for (; *p != '\0'; p++) {
loop_start:
		if (is_escaped) {
			if (lbs_strv_append_char (strv, stri, *p) < 0) {
				goto free_detail;
			}
			is_escaped = false;
			continue;
		}

		if (is_quoted) {
			if (strncmp (p, q[qi].right, qrlen[qi]) == 0) {
				is_quoted = false;
				is_escaped = false;
				ignore_esc = false;
				p += qrlen[qi] - 1;
			} else {
				if (!ignore_esc && strchr (esc, *p)) {
					is_escaped = true;
				} else {
					if (lbs_strv_append_char (strv, stri, *p) < 0) {
						goto free_detail;
					}
				}
			}
			continue;
		}

		if (strchr (delim, *p)) {
			if (is_delimed) {
				continue;
			}
			if (lbs_strv_append_str_empty (strv) < 0) {
				goto free_detail;
			}
			if (detail != NULL && lbs_array_push_back (detail, &(int){-1}) < 0) {
				goto free_detail;
			}
			stri++;
			is_delimed = true;
			continue;
		}
		if (strchr (esc, *p)) {
			is_escaped = true;
			continue;
		}

		is_delimed = false;

		for (int i = 0; i < qlen; i++) {
			if (strncmp (p, q[i].left, qllen[i]) == 0) {
				is_quoted = true;
				ignore_esc = q[i].super;
				qi = i;
				p += qllen[qi]; // p++ will be skipped, so do not minus 1 here
				if (detail != NULL) {
					lbs_array_v (detail, int, stri) = qi;
				}
				goto loop_start; // restart the loop
			}
		}

		if (lbs_strv_append_char (strv, stri, *p) < 0) {
			goto free_detail;
		}
	}

	if (is_delimed && lbs_strv_get_str_len (strv, stri) == 0) {
		lbs_strv_remove_str (strv);
		lbs_array_pop_back (detail);
	}

	if (detail_ptr != NULL) {
		*detail_ptr = detail;
	}

#if __STDC_NO_VLA__
	free (qllen);
	free (qrlen);
#endif
	return strv;


	/* Error-handling goto label */

free_detail:
	if (detail != NULL) {
		lbs_array_unref (detail);
	}

free_strv:
#if __STDC_NO_VLA__
	free (qllen);
	free (qrlen);
#endif
	lbs_strv_unref (strv);
	return NULL;
}


LbsArgQopt* lbs_arg_qopt_new (const char* str) {
	LbsStrv* strv;
	LbsArray* detail;

	strv = lbs_arg_parse (str, ",", "\\", (LbsArgQuote[]) {
		{ "\"", "\"", false }, { "\'", "\'", true }, { NULL, NULL, false}},
		&detail);

	if (strv == NULL || detail == NULL) {
		return NULL;
	}
	/* goto label => free_detail_and_strv */

	size_t strv_len = lbs_strv_get_len (strv);
	LbsArgQopt* qopt = malloc (sizeof (LbsArgQopt) +
		sizeof (LbsArgQoptItem) * (strv_len + 1));
	if (qopt == NULL) {
		goto free_detail_and_strv;
	}

	qopt->len = strv_len;
	qopt->strv = NULL;
	qopt->detail = NULL;
	qopt->opts[strv_len] = (LbsArgQoptItem) { NULL, NULL };

	for (size_t i = 0; i < strv_len; i++) {
		qopt->opts[i].name = lbs_strv_dup_str (strv, i);
		if (qopt->opts[i].name == NULL) {
			goto free_qopt;
		}

		char* pos = strchr (qopt->opts[i].name, '=');
		if (pos == NULL) {
			qopt->opts[i].value = NULL;
		} else {
			*pos = '\0';
			qopt->opts[i].value = pos + 1;
		}

	}

	qopt->strv = strv;
	qopt->detail = detail;
	return qopt;

free_qopt:
	lbs_arg_qopt_free (qopt);

free_detail_and_strv:
	lbs_strv_unref (strv);
	lbs_array_unref (detail);
	return NULL;
}

void lbs_arg_qopt_free_generic (void* qopt_generic) {
	if (qopt_generic == NULL) {
		return;
	}

	LbsArgQopt* qopt = qopt_generic;
	lbs_strv_unref (qopt->strv);
	lbs_array_unref (qopt->detail);

	for (int i = 0; qopt->opts[i].name != NULL; i++) {
		free (qopt->opts[i].name);
	}

	free (qopt);
}
