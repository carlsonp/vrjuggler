/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


/* 
 *
 * Author: Christopher Just
 *
 */

package VjConfig;

import java.util.StringTokenizer;
import java.util.Vector;
import VjConfig.ValType;
import VjConfig.DescEnum;
import java.io.*;

public class PropertyDesc {

    public String name;
    public String token;
    public String help;
    public Vector enums;
    public Vector valuelabels;
    public int num;
    public ValType valtype;



    public PropertyDesc () {
	/* creates an "empty" PropertyDesc */
	name = "";
	token = "";
	help = "";
	enums = new Vector();
	valuelabels = new Vector();
	num = 1;
	valtype = new ValType ("int");
    }



    public PropertyDesc (ConfigStreamTokenizer st) {
	/* note: line is an unparsed configchunk line.  The only 
	 * assumption I'm willing to make about it is that it was
	 * generated by PropertyDesc << on the C++ side.  That
	 * actually lets us assume quite a bit.
	 */
	/* note 2: this gets pretty long & hairy and should probably
	 * be divied up quite a bit
	 */

	try {
	    st.nextToken();
	    token = st.sval;
	    st.nextToken();
	    valtype = new ValType (st.sval);
	    st.nextToken();
	    num = Integer.parseInt(st.sval);
	    st.nextToken();
	    name = st.sval;

	    //System.out.println ("reading property desc: " + name + " " + token);

	    st.nextToken();

	    // note: this next bit with vj_valuelabels and
	    // vj_enumeration tokens needs some cleanup later.
	    // right now vj_enumeration just gets ignored. It
	    // ought to be required, but that would break existing
	    // code, so for now it's just accepted.

	    if ((st.ttype == StreamTokenizer.TT_WORD) 
		&& st.sval.equalsIgnoreCase ("vj_valuelabels")) {
		st.nextToken();
		if (st.ttype != '{') {
		    System.err.println ("Error: parsing " + token + " expected '{' after vj_valuelabels" );
		}
		valuelabels = parseEnumerations (st, new ValType("string"));
	    }
	    else
		valuelabels = new Vector();

	    if ((st.ttype == StreamTokenizer.TT_WORD) 
		&& st.sval.equalsIgnoreCase ("vj_enumeration"))
		st.nextToken();
	    if (st.ttype == '{') {
		enums = parseEnumerations (st, valtype);
	    }
	    else
		enums = new Vector();

	    help = st.sval;
	    
	}
	catch (IOException e) {
	    System.err.println ("error in PropertyDesc constructor");
	    System.err.println (e);
	}
	catch (NumberFormatException e2) {
	    System.err.println ("PropertyDesc.<init>(): Invalid number format: " 
				+ st.sval);
	}
    }



    public DescEnum getEnumAtIndex (int ind) {
	return (DescEnum)enums.elementAt(ind);
    }



    public boolean equals(PropertyDesc d) {
	DescEnum e1, e2;
	if (d == null)
	  return false;
	if (!name.equalsIgnoreCase(d.name))
	    return false;
	if (!token.equalsIgnoreCase(d.token))
	    return false;
	if (!help.equalsIgnoreCase(d.help))
	    return false;
	if (num != d.num)
	    return false;
	if (!valtype.equals(d.valtype))
	    return false;

	/* KLUDGE: This next part returns false if both
	 * PropertyDescs have the same descenums in a
	 * different order.  I'm not sure if the enums
	 * code enforces any kind of order or not, or
	 * if this is reasonable behavior. This should
	 * be examined more carefully.
	 */
	for (int i = 0; i <enums.size(); i++) {
	    try {
		e1 = (DescEnum)enums.elementAt(i);
		e2 = (DescEnum)d.enums.elementAt(i);
		if (!e1.equals(e2))
		    return false;
	    }
	    catch (ArrayIndexOutOfBoundsException e) {
		return false;
	    }
	}
	return true;
    }



    public String toString() {
	String s = token + " " + valtype.toString() + " " 
	    + Integer.toString(num) 
	    + " \"" + name + "\"";

	/* value labels: */
	if (valuelabels.size() > 0) {
	    DescEnum e;
	    s += " vj_valuelabels { ";
	    for (int i = 0; i < valuelabels.size(); i++) {
		e = (DescEnum) valuelabels.elementAt(i);
		s += "\"" + e.str + "\" ";
	    }
	    s += "}";
	}

	/* enumerations */
	if (enums.size() > 0) {
	    DescEnum e;
	    s += " vj_enumeration { ";
	    for (int i = 0; i < enums.size(); i++) {
		e = (DescEnum) enums.elementAt(i);
		if (valtype.equals(ValType.t_string) || 
		    valtype.equals(ValType.t_chunk) ||
		    valtype.equals(ValType.t_embeddedchunk))
		    s += "\"" + e.str + "\" ";
		else
		    s += "\"" + e.str + "=" + e.val + "\" ";
	    }
	    s += "}";
	}
	s += " \"" + help + "\"";
	return s;
    }



    public VarValue getEnumValue(String val) 
	throws java.util.NoSuchElementException {
	/* returns the int value associated with this enum el */
	DescEnum t;
	VarValue v;
	
	if (valtype.equals(ValType.t_string) || 
	    valtype.equals(ValType.t_chunk)) {
	    v = new VarValue(valtype);
	    v.set(val);
	    return v;   // no need for translation
	}
	
	for (int i = 0; i < enums.size(); i++) {
	    t = (DescEnum)enums.elementAt(i);
	    if (t.str.equalsIgnoreCase(val)) {
		v = new VarValue(t.val);
		return v;
	    }
	}
	throw new java.util.NoSuchElementException();
    }



    public String getEnumString(int val) 
	throws java.util.NoSuchElementException {
	/* does the reverse mapping of getEnumVal - maps a value 
	 * back to the name of the enum entry 
	 */
	DescEnum t;

	if (!valtype.equals(ValType.t_int))
	    throw new java.util.NoSuchElementException();
	for (int i = 0; i < enums.size(); i++) {
	    t = (DescEnum)enums.elementAt(i);
	    if (t.val.getInt() == val) {
		return t.str;
	    }
	}
	throw new java.util.NoSuchElementException();
    }



    public String getEnumString(float val) 
	throws java.util.NoSuchElementException {
	/* does the reverse mapping of getEnumVal - maps a 
	 * value back to the name of the enum entry 
	 */
	DescEnum t;

	if (!valtype.equals(ValType.t_float))
	    throw new java.util.NoSuchElementException();
	for (int i = 0; i < enums.size(); i++) {
	    t = (DescEnum)enums.elementAt(i);
	    if (t.val.getFloat() == val) {
		return t.str;
	    }
	}
	throw new java.util.NoSuchElementException();
    }



    private Vector parseEnumerations (ConfigStreamTokenizer st,
				      ValType vt) {
	/* Parses a list of enumerations or valuelabels from
	 * st.  We assume that the opening '{' has already
	 * been read, and we go until we read and consume the
	 * closing '}'
	 */
	DescEnum d;

	try {
	    Vector v = new Vector();

	    /* we've got an enumeration to parse */
	    st.nextToken();
	    int j, enumval = 0;
	    float enumfloatval = 0.0f;
	    int k=0;
	    while (st.ttype != '}') {
		
		//System.out.println (st.sval);
		if (st == null) System.err.println ("foo");
		k++;
		j = st.sval.indexOf('=');
		if (j == -1) {
		    /* no explicit value */
		    if (vt.equals (ValType.t_string) || vt.equals (ValType.t_chunk)
			|| vt.equals (ValType.t_embeddedchunk))
			d = new DescEnum (st.sval, st.sval);
		    else
			d = new DescEnum (st.sval, enumval++);
		    v.addElement (new DescEnum(d));
		}
		else {
		    /* explicit value */
		    VarValue val = new VarValue(vt);
		    String n = st.sval.substring(0,j);
		    val.set (st.sval.substring(j+1));
		    d = new DescEnum (n, val);
		    v.addElement (d);
		}
		st.nextToken();
	    }
	    st.nextToken();
	    return v;
	}
	catch (IOException e) {
	    System.err.println ("error in ParseEnumerations");
	    System.err.println (e);
	}
// 	catch (NumberFormatException e2) {
// 	    System.err.println ("PropertyDesc.<init>(): Invalid" 
// 				+ " number format: " + st.sval);
// 	    System.err.println (e2);
// 	}

	return new Vector(); // if we had an exception
    }


}


