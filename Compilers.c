

#include <stdio.h>
#include <stdlib.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif


int main(int argc, char** argv) {
	int i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_READER, "] - Reader\n* [",
			PGM_SCANNER, "] - Scanner\n* [",
			PGM_PARSER, "] - Parser\n");
		return EXIT_FAILURE;
	}
	Plex_char option = argv[1][0];
	switch (option) {
	case PGM_READER:
		printf("%s%c%s", "\n[Option '", PGM_READER, "': Starting READER .....]\n\n");
		mainReader(argc, argv);
		break;
	case PGM_SCANNER:
		printf("%s%c%s", "\n[Option '", PGM_SCANNER, "': Starting SCANNER .....]\n\n");
		mainScanner(argc, argv);
		break;
	case PGM_PARSER:
		printf("%s%c%s", "\n[Option '", PGM_PARSER, "': Starting PARSER .....]\n\n");
		mainParser(argc, argv);
		break;
	default:
		printf("%s%c%s%c%s%c%s", "* OPTIONS:\n- [",
			PGM_READER, "] - Reader\n- [",
			PGM_SCANNER, "] - Scanner\n- [",
			PGM_PARSER, "] - Parser\n");
		break;
	}
	return EXIT_SUCCESS;
}





Plex_none printLogo() {
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"\t=---------------------------------------=\n",
		"\t|				Rian Olson			   |\n",
		"\t=---------------------------------------=\n",
		"\t|              ....                     |\n",
		"\t|          ........::.::::::.           |\n",
		"\t|        .:........::.:^^^~~~:          |\n",
		"\t|        :^^::::::^^^::^!7??7~^.        |\n",
		"\t|       .:^~~^!77777~~7?YY?7??7^.       |\n",
		"\t|       :.^~!??!^::::^^~!?5PY??!~.      |\n",
		"\t|       ~!!7J~.:::^^^^~!!~~?G5J?~       |\n",
		"\t|       :^~?!~7?Y57^^?PP5YJ!J5Y?:       |\n",
		"\t|       .~!!.:^!7!:.:7JYYJ7~7Y7^        |\n",
		"\t|       .~77..    . .~^:^^^~7?:         |\n",
		"\t|       .^!^~:::.:^!7?~^~!77J:          |\n",
		"\t|        ^^!Y~^^^^~?YJ77??7JJ^          |\n",
		"\t|       .^7J?~^~~^~7??7??7JY?~:         |\n",
		"\t|        ::^^~^7?!^~~!7???J?J7~:.       |\n",
		"\t|         ^~~!.^7YPPPP5Y?7J7777~.       |\n",
		"\t|        ..:~..:^!JPP5YJ?!777!^.        |\n",
		"\t| .~?JJJJJJJJJJYYYYYPPPPPPPPPPPP5PPYY~  |\n",
		"\t|  :!Y5GGG.___ YYYYYY__._.PPGGGGGG5!.   |\n",
		"\t|   :!Y5G / __| ___ / _(_)__ _ PGP5.    |\n",
		"\t|    :~75 \\__ \\/ _ \\  _| / _` | 5?.     |\n",
		"\t|     7~7 |___/\\___/_| |_\\__,_| Y5?.    |\n",
		"\t|    .^~!~.....................P5YY7.   |\n",
		"\t|   .:::::::::::::?JJJJYYYYYYYYYJJJJ7.  |\n",
		"\t|                                       |\n",
		"\t=---------------------------------------=\n"
	);
}
