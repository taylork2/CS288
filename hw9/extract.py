# CS288 Homework 9
# Read the skeleton code carefully and try to follow the structure
# You may modify the code but try to stay within the framework.

import libxml2
import syselm.nodeType -> returns an integer # 3=text and 4=cdata
import os
import commands
import re
import sys

import MySQLdb

from xml.dom.minidom import parse, parseString

# for converting dict to xml 
from cStringIO import StringIO
from xml.parsers import expat

def get_elms_for_atr_val(dom,tag,atr,val):
   lst=[]
   elms = dom.getElementsByTagName(tag)
   for elm in elms:
      if elm.getAttribute(atr) == val:
         lst = elm.childNodes
         return lst

# get all text recursively to the bottom
def get_text(e):
   lst=[]
   if (e.nodeType in (3,4)):
      lst.append(e.data)
   else:
      for x in e.childNodes:
         lst=lst+get_text(x)
   return lst

# replace whitespace chars
def replace_white_space(str):
   p = re.compile(r'\s+')
   new = p.sub(' ',str)   # a lot of \n\t\t\t\t\t\t
   return new.strip()

# replace but these chars including ':'
def replace_non_alpha_numeric(s):
   p = re.compile(r'[^a-zA-Z0-9:-]+')
   #   p = re.compile(r'\W+') # replace whitespace chars
   new = p.sub(' ',s)
   return new.strip()

# convert to xhtml
# use: java -jar tagsoup-1.2.jar --files html_file
def html_to_xml(fn):
   os.system("java -jar tagsoup-1.2.jar --files %s" % (fn))
   xhtml_file=fn.replace('.html', '.xhtml')   
   return xhtml_file

def extract_values(dm):
   lst = []
   l = get_elms_for_atr_val(dm, 'table','class','mdcTable')

   for e in l:
      lst.append(get_text(e))
   return lst

# mysql> describe most_active;
def insert_to_db(l,tbl):
   # ............
   conn = MySQLdb.connect(host='localhost', user='root', passwd='CS288password', db='stock_market')
   curs = conn.cursor()
   s='DROP TABLE IF EXISTS ' + tbl + ';'
   curs.execute(s)
   curs.execute(
       """CREATE TABLE `%s` (
       ticker varchar(20),
       name varchar(50),
       price FLOAT(20),
       changed FLOAT(20),
       pchange FLOAT(20),
       volume INT(50)
       )""" % (tbl))

   for d in l:
      query = "INSERT INTO `%s` (ticker, name, price, changed, pchange, volume) VALUES('%s','%s','%s','%s','%s','%s');" % (tbl, d['symbol'], d['name'],  d['price'], d['change'], d['pchange'], d['volume'])
      curs.execute(query)
      conn.commit()
   cursor.close()
   conn.close() 
   return 0


def tr_to_dict(x):
   d={}
   lst=x[2].split('(')
   d['name']=lst[0]
   d['symbol']=lst[1].replace(')','')
   d['volume']=x[4].replace(',','')
   d['price']=x[5].replace('$','')
   d['change']=x[6]
   d['pchange']=x[7]
   return d


# show databases;
# show tables;
def main():
   html_fn = sys.argv[1]
   fn = html_fn.replace('.html','')
   xhtml_fn = html_to_xml(html_fn)

   # global dom
   dom = parse(xhtml_fn)

   lst = extract_values(dom)
   lstofdict=[]
   for x in range(1, len(lst)):
      lstofdict.append(tr_to_dict(lst[x]))
   
   # make sure your mysql server is up and running
   cursor = insert_to_db(lstofdict,fn) # fn = table name for mysql

   # l = select_from_db(cursor,fn) # display the table on the screen

   # make sure the Apache web server is up and running
   # write a PHP script to display the table(s) on your browser

   # return xml
# end of main()


if __name__ == "__main__":
   main()

# end of hw7.py