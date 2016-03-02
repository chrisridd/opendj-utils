BBEdit Language Modules
=======================

I've written two language modules for BBEdit (and TextWrangler) that help you
navigate through LDAP Data Interchange Format (LDIF) files.

The first version I wrote was a "Codeless Language Module", basically a bunch
of regular expressions. It understands commenting, and tries to populate the
"function" list with the DN of each entry. This makes it pretty convenient to
move around the LDIF file.

Copy LDIF.plist into ~/Library/Application Support/BBEdit/Language Modules/ (or
~/Library/Application Support/TextWrangler/Language Modules/) and restart
BBEdit/TextWrangler to use the first version.

The line-oriented structure of LDIF means it is impossible to do proper
keyword colouring or to handle wrapped lines in LDIF correctly in a codeless
language module, so I wrote a second version in C++.

Open LDIF.xcodeproj in Xcode (5 or 6) and build it. The target LDIF.bblm will
be somewhere in your ~/Library/Developer/Xcode/DerivedData/ directory. Copy it
into ~/Library/Application Support/BBEdit/Language Modules/ (or
~/Library/Application Support/TextWrangler/Language Modules/). Remove LDIF.plist
if you've installed it first and restart BBEdit/TextWrangler.

If you don't want to build it yourself, a pre-built binary is in the
LDIF.bblm.zip file. Unzip it into the correct Language Modules directory in the
previous paragraph.

This version understands wrapped lines, and does some smarter keyword colouring.
It will decode any base64-encoded DNs before putting them in the function list.

There's an extra codeless language module for easily navigating Java jstack
output. There's no standard filename extension for these.

Install the jstack.plist file into the Language Modules directory as described
above.
