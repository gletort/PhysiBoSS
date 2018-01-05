#!/usr/bin/env python
#
# read a template file to generate a parameter configuration file
#
# Adapted from pre_config.py, Copyright Francois J. Nedelec, 3.2010 - 1.2013
# 

"""
    Generate files from a template by evaluating doubly-bracketed code units
    with the python interperter. The code must be valid in python.

Syntax:
   
    templating.py [-NB_DIGITS] [REPEAT] [DEFINITIONS] TEMPLATE_FILE 
    
Description:
    
    The names of the files produced are build from the name of the template
    by inserting a constant-width integer (eg. parameters.csx.tpl -> parameters0001.csx)
    
    The size of the insert can be specified with NB_DIGITS (default=4)
    REPEAT is optional should be a positive integer (default=1).
    If REPEAT is specified, this multiplies the number of output files.

    DEFINITIONS can be given on the command line as 'name=value', 
    of 'name=sequence', with no space around the '='.

Example 1:
    
    Template:
        diffusion_rate = [[ random.uniform(0,1) ]]

    Command:
        templating.py 100 param.csx.tpl
    
"""

import sys

# output stream for error messages:
out = sys.stderr

try:
    import os, cStringIO, copy
    libraries = { 'random': __import__('random'), 'math': __import__('math') }
except ImportError, e:
    out.write("Error: could not load necessary python modules\n")
    sys.exit()


#------------------------------------------------------------------------

def pop_sequence(dict):
    """
        remove an entry in the dictionary with multiple values
    """
    for k in dict:
        v = dict[k]
        try:
            len(v)
            dict.pop(k)
            return (k, v)
        except:
            pass
    return ('', [])


def try_assignment(cmd, values):
    """
        try to evaluate `cmd` as a variable assignent,
        and add the result into values
    """
    [key, equal, value] = cmd.partition('=')
    key = key.strip()
    if key and value:
        try:
            val = eval(value, libraries, values);
            #print "Assignment: ", key, equal, val
            values[key] = val
            return True
        except:
            pass
    return False
    #exec(cmd, libraries, values)


def evaluate(cmd, values):
    """
        evaluate `cmd` and return the result
    """
    #do not substitute multi-bracketed expressions:
    if cmd[0] == '[' and cmd[-1] == ']':
        return cmd
    return eval(cmd, libraries, values)


def get_block(file, s, e):
    """
    Extract the next block starting with DOUBLE delimiters 'ss' and ending with 'ee'
    Returns a set with 3 values:
        - the text found before the block
        - the block with its delimiters
        - EOF (True or False)
    """
    pre = cStringIO.StringIO()
    blk = cStringIO.StringIO()
    str = pre
    
    ch = file.read(1)
    depth = 0
    dep = 0
    while ch:
        pc = ch
        ch = file.read(1)
        if pc == s:
            depth += 1
            if ch == s:
                str = blk
                dep = depth
        str.write(pc)
        if pc == e:
            depth -= 1
            if ch == e and depth == dep:
                blk.write(ch)
                return (pre.getvalue(), blk.getvalue(), False)
    if blk.getvalue():
        out.write("Error: EOF found while reading bracketted block\n");
    return (pre.getvalue(), '', True)



#------------------------------------------------------------------------

index = 0
file_parts = ['config', '%04i', '.out']
files_made = []


def set_file_name(path):
    """
        extract the root and the extension of the file
    """
    global file_parts, files_made
    files_made = []
    file = os.path.basename(path)
    [main, ext] = os.path.splitext(file)
    if '.' in main:
        [main, ext] = os.path.splitext(main)
    file_parts[0] = main;
    file_parts[2] = ext;


def next_file_name():
    """
        Generate the name for the next output
    """
    global index, file_parts
    n = file_parts[0] + ( file_parts[1] % index ) + file_parts[2]
    index += 1
    return n


def make_file(content, values):
    """
        Create a file with the specified content
    """
    global files_made
    fname = next_file_name()
    file = open(fname, 'w')
    file.write(content)
    file.close()
    files_made.extend([fname]);
    out.write("\\"+repr(values)+'\n')
    out.write(" \\"+('> '+fname).rjust(78, '-')+'\n')


#------------------------------------------------------------------------

def process(ifile, values={}, content=''):
    """
        parse() will generate output files by substituting
        any bracketed block found in the input file,
        except triply-bracketed blocks.
    """
    
    #create a mem-file with the content
    ofile = cStringIO.StringIO()
    ofile.write(content)

    while ifile:
        (pre, cmd, eof) = get_block(ifile, '[', ']')
        ofile.write(pre)
        
        if eof:
            #we have reached the end of the input:
            make_file(ofile.getvalue(), values)
            return
        
        #print "* "+cmd+" *"
        cmd = cmd[2:-2]
    
        # interpret command:
        if try_assignment(cmd, values):
            # check if a variable with multiple values was specified:
            (key, vals) = pop_sequence(values)
            if key:
                val = vals.pop()
                ipos = ifile.tell()
                for v in vals:
                    values[key] = v
                    out.write("|  %35s <-- %s\n" % (key, str(v)) )
                    process(ifile, values, ofile.getvalue())
                    ifile.seek(ipos)
                out.write("|  %35s <-- %s\n" % (key, str(val)) )
                values[key] = val
        else:
            try:
                vals = evaluate(cmd, values)
            except StandardError, e:
                out.write("Syntax error, "+e.message+" in :\n");
                out.write("    "+cmd+'\n')
                return
            try:
                # check if multiple values were specified:
                val = vals.pop()
                ipos = ifile.tell()
                for v in vals:
                    out.write("|  %35s --> %s\n" % (cmd, str(v)) )
                    process(ifile, values, ofile.getvalue()+str(v))
                    ifile.seek(ipos)
            except (AttributeError, IndexError), e:
                # handle a single value:
                val = vals
            out.write("|  %35s --> %s\n" % (cmd, str(val)) )
            ofile.write(str(val))


def expand(ifile, values):
    """
        Call self recursively to remove all entries of dictionary
        'values' that are associated with multiple keys
    """
    (key, vals) = pop_sequence(values)
    if key:
        ipos = ifile.tell()
        for v in vals:
            values[key] = v
            #out.write("|  %35s <-- %s\n" % (key, str(v)) )
            expand(ifile, values)
            ifile.seek(ipos)
        #restore multi-values for upward recursion
        values[key] = vals
    else:
        return process(ifile, values, '')


def parse(iname, values={}, repeat=1):
    """
        process one file, and return a list of generated files
    """
    global files_made
    set_file_name(iname)
    for x in range(repeat):
        f = open(iname, 'r')
        expand(f, values)
        f.close()
    return files_made


#------------------------------------------------------------------------


def command(args):
    inputs = []
    repeat = 1
    values = {}
    
    for arg in args:
        if os.path.isfile(arg):
            inputs.append(arg)
        elif arg.isdigit():
            repeat = int(arg)
        elif arg[0] == '-' and arg[1:].isdigit():
            global file_parts
            nd = int(arg[1:])
            file_parts[1] = '%%%03ii' % nd
        elif try_assignment(arg, values):
            pass
        else:
            out.write("  Error: unexpected argument `%s'\n" % arg)
            sys.exit()
    
    if not inputs:
        out.write("  Error: you must specify an input template file\n")
        sys.exit()

    for i in inputs:
        out.write("reading %s\n" % i)
        res = parse(i, values, repeat)
        out.write("Generated %i files from %s\n" % (len(res), i))
        #out.write(repr(res))



#------------------------------------------------------------------------

if __name__ == "__main__":
    if len(sys.argv) < 2 or sys.argv[1] == '-h' or sys.argv[1] == 'help':
        print(__doc__)
    else:
        command(sys.argv[1:])


