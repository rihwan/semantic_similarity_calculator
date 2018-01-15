# [Open Source Node.js Addon] Semantic Similarity Calculator (used in DoAnim.com)

<b>Features:</b>
* MIT license (WordNet has its own license term, please read below)
* Thread-Safe
* Improved performance by accessing synset files from memory 
* Distance metric between two sets of words (take into account synonyms)
* Database files and source code for searching synonyms from WordNet 3.1 (https://wordnet.princeton.edu/)
* Inspired by "Sentence similarity based on semantic nets and corpus statistics" (http://ieeexplore.ieee.org/document/1644735/)

<b>Prerequisite:</b>
* Please download WordNet 3.1 and unzip it in ssc/dict directory (http://wordnetcode.princeton.edu/wn3.1.dict.tar.gz)
* Please download Node.JS header and library files and locate it in dependencies/node-v4.5.0
* Boost 1.62.0

<b>WordNet License:</b>
WordNet Release 3.0 This software and database is being provided to you, the LICENSEE, by Princeton University under the following license. By obtaining, using and/or copying this software and database, you agree that you have read, understood, and will comply with these terms and conditions.: Permission to use, copy, modify and distribute this software and database and its documentation for any purpose and without fee or royalty is hereby granted, provided that you agree to comply with the following copyright notice and statements, including the disclaimer, and that the same appear on ALL copies of the software, database and documentation, including modifications that you make for internal use or for distribution. WordNet 3.0 Copyright 2006 by Princeton University. All rights reserved. THIS SOFTWARE AND DATABASE IS PROVIDED "AS IS" AND PRINCETON UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED. BY WAY OF EXAMPLE, BUT NOT LIMITATION, PRINCETON UNIVERSITY MAKES NO REPRESENTATIONS OR WARRANTIES OF MERCHANT- ABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE, DATABASE OR DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS, COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS. The name of Princeton University or Princeton may not be used in advertising or publicity pertaining to distribution of the software and/or database. Title to copyright in this software, database and any associated documentation shall at all times remain with Princeton University and LICENSEE agrees to preserve same.