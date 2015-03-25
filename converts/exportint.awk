BEGIN{ORS=""; print "TIMEOUT 10\nSPEED 57600\n";type=""; w=0; wmax=32;}
function foo(k, str){
	if (type!=k) {
		if (type!="") print "\nEND\n"
		print str"\n";
		w=0;
	}
	type=$1;
	sub(/^0[xX]/,"", $2);
	$2=toupper($2);
	w+=length($2);
	if (w>wmax) {print "\n";w=length($2);}
	print $2;
}
/^>/{ foo($1, "EXPECT"); }
/^</{ foo($1, "SEND"); }
END{if (type!="") print "\nEND"}
