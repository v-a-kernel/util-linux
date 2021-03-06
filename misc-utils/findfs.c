/*
 * Copyright (C) 2009 Karel Zak <kzak@redhat.com>
 *
 * This file may be redistributed under the terms of the GNU Public
 * License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include <blkid.h>

#include "nls.h"
#include "closestream.h"
#include "c.h"
#include "exitcodes.h"

/* Exit codes used by findfs. */
#define FINDFS_SUCCESS		0	/* no errors */
#define FINDFS_NOT_FOUND	1	/* label or uuid cannot be found */
#define FINDFS_USAGE_ERROR	2	/* user did something unexpected */

static void __attribute__((__noreturn__)) usage(int rc)
{
	FILE *out = rc ? stderr : stdout;
	fputs(USAGE_HEADER, out);
	fprintf(out, _(" %s [options] {LABEL,UUID,PARTUUID,PARTLABEL}=<value>\n"),
		program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Find a filesystem by label or UUID.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);
	fprintf(out, USAGE_MAN_TAIL("findfs(8)"));
	exit(rc);
}

int main(int argc, char **argv)
{
	char	*dev;
	int	c;
	static const struct option longopts[] = {
		{"version", no_argument, NULL, 'V'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	atexit(close_stdout);

	if (argc != 2)
		/* we return '2' for backward compatibility
		 * with version from e2fsprogs */
		usage(FINDFS_USAGE_ERROR);

	while ((c = getopt_long(argc, argv, "Vh", longopts, NULL)) != -1)
		switch (c) {
		case 'V':
			printf(UTIL_LINUX_VERSION);
			return EXIT_SUCCESS;
		case 'h':
			usage(FINDFS_SUCCESS);
		default:
			errtryhelp(EXIT_FAILURE);
		}

	dev = blkid_evaluate_tag(argv[1], NULL, NULL);
	if (!dev)
		errx(FINDFS_NOT_FOUND, _("unable to resolve '%s'"), argv[1]);

	puts(dev);
	return FINDFS_SUCCESS;
}

