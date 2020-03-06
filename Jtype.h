using namespace std;

class Jtype 
{
private:
	bitset<6> op;
	bitset<26> tar;
public:
	void setValue(int _op, int _tar) {
		op = bitset<6>(_op);
		tar = bitset<26>(_tar);
	}

	string binCode() {
		return op.to_string() + tar.to_string();
	}
};
