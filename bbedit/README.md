BBEdit Language Module
======================

This is a BBEdit (and TextWrangler) Codeless Language Module that helps you
navigate through LDAP Data Interchange Format (LDIF) files.

Copy the file into ~/Library/Application Support/BBEdit/Language Modules/ (or
~/Library/Application Support/TextWrangler/Language Modules/) and restart
BBEdit/TextWrangler.

It really only does two things at the moment. It understands commenting, and
(more usefully) it populates the "function" list with the DN of each entry in
the file. This makes moving around the file quite convenient.

The line-oriented structure of LDIF means it is impossible to do proper
keyword colouring or to handle wrapped lines in LDIF correctly in a codeless
language module.
