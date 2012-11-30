Convert from svn to git
=======================

Currently the OpenDJ repository is held in subversion. This is a set of scripts
using the svn-all-fast-export tool from KDE that convert it (one-way) to a
nice clean git repository.

I originally discovered this tool [here](http://blog.smartbear.com/software-quality/bid/170525/Migrating-from-Subversion-to-Git-Lessons-Learned)
after struggling with git-svn.

That tool is pretty difficult to build if you don't have exactly the right
versions of KDE/Qt/etc, so for simplicity I run this in Ubuntu 12. The
converted repo obviously doesn't require Linux.

The first thing you want is a local copy of the whole SVN repo. I put this in
opendj-svn. This has all the branches, tags, everything.

Then just run the `convert` script. It takes a few minutes, and uses the rules
in `rules` to get around various oddities in the svn repo. This is quite amazing
because git-svn takes about 10 hours to do the same conversion!

The main difficulty in the conversion is handled by running the tool in several
steps. It does this because the committer names used by svn should be converted
into "correct" email addresses for git, and over time the same committer names
have worked for Sun, Oracle, and ForgeRock. I set up 3 `idmap.*` files to cover
these three periods of time. I don't think the boundaries between all three
periods is completely correct. Corrections are welcomed!
