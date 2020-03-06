using namespace std;

class Rtype 
{
private:
	bitset<6> op;
	bitset<5> rs;
	bitset<5> rt;
	bitset<5> rd;
	bitset<5> shamt;
	bitset<6> funct;

public:
	void setValue(int _op, int _rs, int _rt, int _rd, int _shamt, int _funct){
		op = bitset<6>(_op);
		rs = bitset<5>(_rs);
		rt = bitset<5>(_rt);
		rd = bitset<5>(_rd);
		shamt = bitset<5>(_shamt);
		funct = bitset<6>(_funct);
	}
	
	string binCode() {
		return op.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + shamt.to_string() + funct.to_string();
	}
	
};

