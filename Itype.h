using namespace std;

class Itype 
{
private:
	bitset<6> op;
	bitset<5> rs;
	bitset<5> rt;
	bitset<16> im;
public:
	void setValue(int _op, int _rs, int _rt, int _im)  {
		op = bitset<6>(_op);
		rs = bitset<5>(_rs);
		rt = bitset<5>(_rt);
		im = bitset<16>(_im);
	}

	string binCode() {
		return op.to_string() + rs.to_string() + rt.to_string() + im.to_string();
	}
};