#!/usr/bin/python
"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import random, sys
from optparse import OptionParser

def generateTabs(nTabs):
    for i in range(nTabs):
        sys.stdout.write("\t")

def removeNewLine(word):
    newWord = ""
    for letter in word:
        if (letter != "\n"):
            newWord = newWord + letter
    return newWord


class comm:
    def __init__(self, files, options, parser):
        #Constructor for comm
        self.files = files
        self.options = options
        self.parser = parser
        
    def compareLines(self):
        #get lines from each file
        self.newList = []
        f1 = self.files[0].readlines()
        f2 = self.files[1].readlines()

        file1 = []
        file2 = []

        f1ptr = 0
        f2ptr = 0
            

        #process each line and remove new lines
        for word in f1:
            file1.append(removeNewLine(word))
            f1ptr = f1ptr + 1
        for word in f2:
            file2.append(removeNewLine(word))
            f2ptr = f2ptr + 1
        
        f1ptr = 0
        f2ptr = 0
        
        #indicates whether both files are sorted
        #TRUE if both are sorted, FALSE if one or both are not
        sortFlag = (file1 == sorted(file1)) and (file2 == sorted(file2))
        
        #checks if files are unsorted and unsorted option has been chosen
        if not sortFlag and not self.options.unsorted:
            self.parser.error("Files are not sorted, please use the -u flag")
       
        #unsorted
        if self.options.unsorted:
            col1 = []
            col2 = []
            col3 = []
            unorderedWords = []

            for l1 in file1:
                col1.append(l1)


            for l2 in file2:
                col2.append(l2)


            for l1 in file1:
                for l2 in file2:
                    if l1 == l2:
                        if l1 not in col3:
                            flag = min(col2.count(l1), col1.count(l1))
                            if col3.count(l1) < min:
                                if col2.count(l1) < col1.count(l1):
                                    col2.remove(l1)
                                else:
                                    col1.remove(l1)
                                col3.append(l1)
            unorderedWords = col1 + col2
                                
            for word in unorderedWords:
                if word in col3 and not self.options.bothFiles:
                    tabs = 2
                    if self.options.surpress1 and self.options.surpress2:
                        tabs = 0
                    elif self.options.surpress1 or self.options.surpress2:
                        tabs = 1
                    generateTabs(tabs)
                    sys.stdout.write(word+ "\n")
                    col3.remove(word)
                    continue
                if word in col1 and not self.options.surpress1:
                    tabs = 0
                    generateTabs(tabs)
                    sys.stdout.write(word + "\n")
                    col1.remove(word)
                    continue
                if word in col2 and not self.options.surpress2:
                    tabs = 1
                    if self.options.surpress1:
                        tabs = 0
                    generateTabs(tabs)
                    sys.stdout.write(word + "\n")
                    col2.remove(word)
                    continue
        #sorted
        else:  
            #loop through file1 and file2
            while (f1ptr < len(file1) and f2ptr < len(file2)):
                if (file1[f1ptr] < file2[f2ptr]):
                    if not self.options.surpress1:
                        tabs = 0
                        generateTabs(tabs)
                        sys.stdout.write(file1[f1ptr] + "\n")
                    f1ptr = f1ptr+1
                    continue
                if (file2[f2ptr] < file1[f1ptr]):
                    if not self.options.surpress2:
                        tabs = 1
                        if self.options.surpress1:
                            tabs = 0
                        generateTabs(tabs)
                        sys.stdout.write(file2[f2ptr] + "\n")
                    f2ptr = f2ptr + 1
                    continue
                if (file1[f1ptr] == file2[f2ptr]):
                    if not self.options.bothFiles:
                        tabs = 2
                        if self.options.surpress1 and self.options.surpress2:
                            tabs = 0
                        elif self.options.surpress1 or self.options.surpress2:
                            tabs = 1
                        generateTabs(tabs)
                        sys.stdout.write(file1[f1ptr] + "\n")
                    f2ptr = f2ptr + 1
                    f1ptr = f1ptr + 1
                    continue
            if (f1ptr < len(file1)):
                if not self.options.surpress1:
                    tabs = 0
                    generateTabs(tabs)
                    sys.stdout.write(file1[f1ptr] + "\n")
                    f1ptr = f1ptr+1
            if (f2ptr < len(file2)):
                if not self.options.surpress2:
                    tabs = 1
                if self.options.surpress1:
                    tabs = 0
                generateTabs(tabs)
                sys.stdout.write(file2[f2ptr] + "\n")
                f2ptr = f2ptr + 1
                


def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [-123] [-u] file1 file2
    Compares two files
    Options:
    -1 Suppress column 1 (lines unique to FILE1)
    -2 Suppress column 2 (lines unique to FILE2)
    -3 Suppress column 3 (lines common to FILE1 and FILE2)
    -u Compare unsorted input
    """

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-1", action="store_true", dest="surpress1", default=False, help="surpress lines unique to file1")
    parser.add_option("-2", action="store_true", dest="surpress2", default=False, help="surpress lines unique to file2")
    parser.add_option("-3", action="store_true", dest="bothFiles", default=False, help="surpress lines common to both files")
    parser.add_option("-u", action="store_true", dest="unsorted", default=False)
    options, args = parser.parse_args(sys.argv[1:])

    if (len(args) != 2):
        parser.error("wrong number of arguments")
    try:
        f = [open(arg, 'r') if arg != '-' else sys.stdin for arg in args]
    except:
        parser.error("cannot read file")

    c = comm(f, options, parser)
    c.compareLines()

if __name__ == "__main__":
    main()
