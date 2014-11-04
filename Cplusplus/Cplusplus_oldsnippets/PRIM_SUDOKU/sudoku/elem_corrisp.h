class elem_corrisp{
	int start_r,end_r,start_c,end_c;
public:
	elem_corrisp(int sr=0,int er=0,int sc=0,int ec=0):start_r(sr),end_r(er),start_c(sc),end_c(ec){}
	void setter(int sr,int er,int sc,int ec);
	int get_start_r(){ return start_r;}
	int get_end_r(){ return end_r; }
	int get_start_c(){ return start_c; }
	int get_end_c() {return end_c; }
};
