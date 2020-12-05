n,q=chr(10),chr(34)
def A():s="def B():s=A();print('n,q=chr(10),chr(34)',n,'def A():s=',q,s,q,';return s;',n,s,n,'B()',sep='');";return s;
def B():s=A();print('n,q=chr(10),chr(34)',n,'def A():s=',q,s,q,';return s;',n,s,n,'B()',sep='');
B()
