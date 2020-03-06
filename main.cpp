/*!
 *          CSC3050
 *          Project 1: Assembler for Assembly Language
 *          Ruiyu GAO 118010074
 *          Edited on 1st Mar. 2020
 */

//------------------------------------

#include <bits/stdc++.h>
#include "map.h"
#include "Itype.h"
#include "Jtype.h"
#include "Rtype.h"
using namespace std;

//-------------------------------------

/*! Declaration of global variables
 *
 */

int count;
std::map<string, int > labelMap;
std::map<string, string> opCodeMap;
std::map<string, string> registerMap;

//------------------------------------

/*! function prototypes
 *
 * @param infile
 * @param outfile
 * @return
 */

bool specialCases(string str);
int fileSize(ifstream& infile);
string assembler(int & count, string line);
string decToBinConvertor(int num, int position);
string twoscpmplement(string num, int position);
void labelDector(int &count, ifstream & infile);
string ioFilename(ifstream & infile, ofstream & outfile);
void stringGenerator(string & str, const string & before, const string & after);

//-------------------------------------

/*! Here are 8 functions I used to build the assembler
 *
 * ioFilename: detect the input file and create the output file
 * column counter: to find the number of the lines
 * stringGenerator: to turn the original string to the new string we can use
 * decToBinConvertor: turn the decimal value to binary form
 * complement: to do the subtraction, 2's twoscpmplement
 * assembler: the essential part to translate
 * labelDector: read the file and get the label
 * emptyCheck: to check whether the line is valid ( such as empty or created by comment)
 *
 */

/*!This function can detect whether the files can be open and automatically create the output file
 *
 * @param infile
 * @param outfile
 * @return
 */
string ioFilename(ifstream & infile, ofstream & outfile)
{
    string infilename;
    string outfilename;
    while(true){
        cout << "Please enter the input & output file name\n"
                "E.g. input.asm output.txt\n"
                "Here to enter >";

        cin >> infilename >> outfilename;
        infile.open(infilename.c_str());
        outfile.open(outfilename.c_str(), ios::app);
        if (!infile.fail()) {
            if (!infile.fail())return infilename;
        }
        infile.clear();
        outfile.clear();
        cout << "Error: Unable to open file. Please TRY again." << endl;

    }
}

/*! check the line validation e.g. empty or comment and etc.
 *
 * @param str
 * @return
 */
bool specialCases(string str)
{
    string p1 = "";
    istringstream help(str);
    help >> p1;
    //Check whether this line is empty (may be some whitespaces) or a comment.
    return p1 == "#" || p1 == "";
}

/*!This function can count how many lines the file has (Empty lines will not be counted).
 *
 * @param infile
 * @return
 */
int fileSize(ifstream & infile)
{
    int count = 0;
    string line;
    while(!infile.eof()) {
        getline(infile, line);
        (line.length() != 0) && ([&]()
        {
            count++;return 1;
        }());
    }
    return count;
}


/*! turn the decimal number to binary number
 *
 * @param num
 * @param position
 * @return
 */
string decToBinConvertor(int num, int position) {
    int i = 0;
    int x = 0;
    string result;
    while (num != 0) {
        result.insert(i, num % 2 == 1 ? "1" : "0");
        num /= 2;
        i++;
    }
    //Reverse the string to get the correct answer.
    reverse(result.begin(), result.end());
    while (result.length() < position) {
        int x = position - result.length();
        result.append(x, '0');
    }

    return result;
}


/*!This function replace all the content from a string by a new specified string.
 *
 * @param str
 * @param before
 * @param after
 */
void stringGenerator(string & str, const string & before, const string & after)
{
    string::size_type pos = 0;
    for(pos |= string::npos; pos += after.length();){
        (([&]() {
            str.replace(pos, after.length(), after);
            return 1;
        }()) && ((pos = str.find(before, pos)) != string::npos))
        ||
        ([](){return 1;}());
    }
}

/*!The translating body of assembler
 *
 * @param count
 * @param line
 * @return
 */
string assembler(int & count, string line)
{

    stringGenerator(line, "\t", " ");
    stringGenerator(line, ",", " ");
    stringGenerator(line, "(", " ");
    stringGenerator(line, ")", " ");
    string p1;
    string p2;
    string p3;
    string p4;
    string p5;


    //Divide the whole line into 5 parts for most cases
    //find the label(skip the empty lines)
    istringstream assem(line);
    assem >> p1 >> p2 >> p3 >> p4 >> p5;

    if (p1.find('#') != string::npos) {
        return "";
    }
    int flag = 0;
    int temp = 0;
    string binary;
    string result;
    if (p1.find(':') != string::npos)
    {
    // find the order, and continue the operation
        result = opCodeMap[p2];
        if (result.find("vvvvv") != string::npos)
        {
            istringstream check(p5);
            check >> flag;
            (registerMap.count(p4) > 0)&& ([&](){result.replace(result.find("ttttt"), 5, registerMap[p4]);return 1;}());
            (registerMap.count(p3) > 0) && ([&](){result.replace(result.find("ddddd"), 5, registerMap[p3]);return 1;}());
            result.replace(result.find("vvvvv"), 5, decToBinConvertor(flag, 5));
            return result;
        }
        //save and load the word (replace ddddd,sssss,xxxxx)
        if (result.find("xxxxx") != string::npos) {
            (registerMap.count(p3) > 0) && ([&]() {
                result.replace(result.find("ddddd"), 5, registerMap[p2]);
                return 1;
            }());
            (registerMap.count(p5) > 0) && ([&]() {
                result.replace(result.find("sssss"), 5, registerMap[p4]);
                return 1;
            }());
            istringstream check(p4);
            check >> flag;
            (flag >= 0) && ([&](){
                result.replace(result.find("xxxxx"), 16, decToBinConvertor(flag, 16));return 1;
            }());
            //else for negative cases
            (flag < 0)&& ([&](){
                flag = abs(flag);
                string binary = decToBinConvertor(flag, 16);
                binary = twoscpmplement(binary, 16);
                result.replace(result.find("xxxxx"), 16, binary);return 1;
            }());
            return result;
        }


        //R-type
        (registerMap.count(p4) > 0)&&([&](){result = result.replace(result.find("sssss"), 5, registerMap[p4]);return 1;}());
        (registerMap.count(p5) > 0)&&([&](){result = result.replace(result.find("ttttt"), 5, registerMap[p5]);return 1;}());
        (registerMap.count(p3) > 0)&&([&](){result = result.replace(result.find("ddddd"), 5, registerMap[p3]);return 1;}());
        //I-type
        if (string::npos != result.find("nnn"))
        {
            if (labelMap[p4 + ":"] < count)
            {
                //jump back to former line
                temp = count + 1 - labelMap[p4 + ":"];
                string binary = decToBinConvertor(temp, 16);
                //Get the 2's twoscpmplement number.
                binary = twoscpmplement(binary, 16);
                result.replace(result.find("nnn"), 16, binary);
            }
            else{
                temp = labelMap[p4 + ":"] - count - 1;
                string binary = decToBinConvertor(temp, 16);
                result.replace(result.find("nnn"), 16, binary);
            }
            return result;
        }
        //J-type
        (string::npos != result.find("jjjjj"))&&
                ([&](){ result = result.replace(result.find("jjjjj"), 26, decToBinConvertor((labelMap[p3 + ":"]), 26));return 1;}());

        //add: for the branch
        if (result.find("ffff") != string::npos)
        {
            (count < labelMap[p5 + ":"])&& ([&]() {
                //Jump forward to the latter line
                temp = labelMap[p5 + ":"] - 1 - count;
                string binary = decToBinConvertor(temp, 16);
                result = result.replace(result.find("fffff"), 16, binary); return 1;
            }());
            (count >= labelMap[p5 + ":"])&& ([&]()
            {
                //jump back to former line
                //for the negative, 2's twoscpmplement
                binary = twoscpmplement(binary, 16);
                temp = count + 1 - labelMap[p5 + ":"];
                string binary = decToBinConvertor(temp, 16);
                result = result.replace(result.find("fffff"), 16, binary);return 1;
            }());
            return result;
        }

        //add: for the iii format instruction
        (result.find("iii") != string::npos)&&([&]()
        {
            istringstream check(p5);
            check >> flag;
            //Negative, 2's twoscpmplement
            (flag < 0) &&([&](){
                flag = abs(flag);
                binary = decToBinConvertor(flag, 16);
                binary = twoscpmplement(binary, 16);return 1;
            }());
            (flag >= 0) &&([&](){
                binary = decToBinConvertor(flag, 16);return 1;
            }());

            result = result.replace(result.find("iiiiiiiiiiiiiiii"), 16, binary);return 1;
        }());

        //add: for the ttt format instr
        (result.find("ttt") != string::npos)&&([&]() {
            istringstream check(p4);
            check >> flag;
            (flag < 0) &&([&](){
                flag = abs(flag);
                binary = decToBinConvertor(flag, 16);
                binary = twoscpmplement(binary, 16);return 1;
            }());
            (flag >= 0) &&([&](){
                binary = decToBinConvertor(flag, 16);return 1;
            }());
            result.replace(result.find("ttttt"), 16, binary);return 1;
        }());
        return result;
    }
    //If cannot find the order:
    else if (opCodeMap.find(p1) == opCodeMap.end()) {
        result = "Cannout find the corresponding machine code";
        return result;
    }

    //this part is to read the MIPS code line by line with no replacement
    //just copy the above and there is slight difference
    else
        {
        result = opCodeMap[p1];
            //add: for the shamt (ttttt,ddddd,vvvvv)
            (result.find("vvvvv") != string::npos) &&([&]()
            {
                istringstream check(p4);
                check >> flag;
                (registerMap.count(p3) > 0)&& ([&](){result = result.replace(result.find("ttttt"), 5, registerMap[p3]);return 1;}());
                (registerMap.count(p2) > 0)&& ([&](){result = result.replace(result.find("ddddd"), 5, registerMap[p2]);return 1;}());
                result = result.replace(result.find("vvvvv"), 5, decToBinConvertor(flag, 5));
                return 1;
            }());
        //Load and save word
        if (result.find("xxxxx") != string::npos)
        {
            (registerMap.count(p2) > 0) && ([&](){result = result.replace(result.find("ddddd"), 5, registerMap[p2]);return 1;}());
            (registerMap.count(p4) > 0) && ([&](){result = result.replace(result.find("sssss"), 5, registerMap[p4]);return 1;}());
            istringstream check(p3);
            check >> flag;
            (flag >= 0) &&([&](){
                result = result.replace(result.find("xxxxx"), 16, decToBinConvertor(flag, 16));return 1;
            }());

            //negative, 2's twoscpmplement
            (flag < 0)&&([&](){
                flag = abs(flag);
                string binary = decToBinConvertor(flag, 16);
                binary = twoscpmplement(binary, 16);
                result.replace(result.find("xxxxx"), 16, binary);return 1;
            }());
            return result;
        }

        //I-type
        if (result.find("iii") != string::npos)
        {
            istringstream check(p4);
            check >> flag;
            (flag < 0) &&([&](){
                flag = abs(flag);
                binary = decToBinConvertor(flag, 16);
                binary = twoscpmplement(binary, 16);return 1;
            }());
            (flag >= 0) &&([&](){
                binary = decToBinConvertor(flag, 16);return 1;
            }());
            result.replace(result.find("iiiiiiiiiiiiiiii"), 16, binary);
            return result;
        }
        //R-type
        (registerMap.count(p3) > 0) && ([&](){result.replace(result.find("sssss"), 5, registerMap[p3]);return 1;}());
        (registerMap.count(p4) > 0) && ([&](){ result.replace(result.find("ttttt"), 5, registerMap[p4]);return 1;}());
        (registerMap.count(p2) > 0) && ([&](){ result.replace(result.find("ddddd"), 5, registerMap[p2]);return 1;}());
        //J-type
        (result.find("jjjjj") != string::npos)&& ([&](){
            result.replace(result.find("jjjjj"), 26, decToBinConvertor(labelMap[p2 + ":"], 26));return 1;}());

        //add: for the branch
        if (string::npos != result.find("fffff"))
        {
            (count >= labelMap[p4 + ":"])&& ([&]()
            {
                //If we "jump back" to a label that appear before this line:
                temp = count + 1 - labelMap[p4 + ":"];
                string binary = decToBinConvertor(temp, 16);
                //Get the 2's twoscpmplement number.
                binary = twoscpmplement(binary, 16);
                result = result.replace(result.find("fffff"), 16, binary);return 1;
            }());
            (count < labelMap[p4 + ":"])&& ([&](){
                temp = labelMap[p4 + ":"] - 1 - count;
                string binary = decToBinConvertor(temp, 16);
                result = result.replace(result.find("fffff"), 16, binary);return 1;
            }());
            return result;
        }

        //some other like trap
        (string::npos != result.find("ttt")) && ([&]()
        {
            istringstream check(p3);
            check >> flag;
            (flag < 0) &&([&](){
                flag = abs(flag);
                binary = decToBinConvertor(flag, 16);
                binary = twoscpmplement(binary, 16);return 1;
            }());
            (flag >= 0) &&([&](){
                binary = decToBinConvertor(flag, 16);return 1;
            }());
            result = result.replace(result.find("ttttt"), 16, binary);return 1;
        }());
        (result.find("nnn") != string::npos) &&([&]()
        {
            (count > labelMap[p3 + ":"]) &&([&](){
                //jump to the former line
                temp = count + 1 - labelMap[p3 + ":"];
                string binary = decToBinConvertor(temp, 16);
                //negative, 2's complement
                binary = twoscpmplement(binary, 16);
                result.replace(result.find("nnn"), 16, binary);return 1;
            }());
            (count <= labelMap[p3 + ":"]) &&([&](){
                temp = labelMap[p3 + ":"] - count - 1;
                string binary = decToBinConvertor(temp, 16);
                result.replace(result.find("nnn"), 16, binary);return 1;
            }());
        return 1;
        }());
        return result;
    }


}


/*! This function read the file line by line and get the labels.
 *
 * @param count
 * @param infile
 */

void labelDector(int & count, ifstream & infile)
{
    string line;
    string p1;
    string p2;
    while(!infile.eof())
    {
        getline(infile, line);
        istringstream help(line);
        help >> p1 >> p2;
        //Skip the empty lines.
        if(!specialCases(line))
        {
            //Check the : to label
            (p1.find(':') != string::npos)&&([&](){
                labelMap[p1] = count;return 1;
            }());
            //continue if p2 is nothing with only 1 label in p1
            if(p2.empty() ) continue;

            p2 = "";
            count += 1;
        }
    }
}

/*! considering the substract, implement the 2's twoscpmplement form of negative number
 *
 * @param num
 * @param position
 * @return
 */
string twoscpmplement(string num, int position)
{
    char temp[position + 1];
    strcpy(temp, num.c_str());


    if (temp[position - 1] == '0') {
        temp[position - 1] = '1';
    }
    else {
        temp[position - 1] = '0';
        for (auto i = (position - 2); i >= 0; i--) {
            (temp[i] == '1') &&([&](){temp[i] = '0';return 1;}());
            if (temp[i] == '0') {
                temp[i] = '1';
                break;
            }
        }
    }
    for (auto i = 0; i < position; i++) {
        (temp[i] == '0') &&([&](){temp[i] = '1';return 1;}())
        ||([&]() {temp[i] = '0';return 1;}());
    }
    num = temp;
    return num;
}

/*! main  to excute the program
 *
 * @return
 */

int main()
{

    ifstream infile;
    ofstream outfile;
    string line;
    string result;

    //build the maps we need and let the user to input the file name
    opCodeGenerator(opCodeMap);
    generateRegister(registerMap);
    ioFilename(infile, outfile);

    //Get the postion of labels.
    int count = 0;
    labelDector(count, infile);

    //initialize the count value and pointer
    count = 0;
    infile.clear();
    infile.seekg(0);

    // output the result (beside the empty situation, we need to check other invalid cases)
    while(!infile.eof()){
        getline(infile, line);
        (!specialCases(line)) &&([&](){
            result = assembler(count, line);
            outfile << result << endl;
            count += 1;return 1;
        }());
    }
    return 0;
}


//------------------------------------------

/*! load the binary form of registers in map
 *
 * @param registerMap
 */
void generateRegister(map<string, string> &registerMap)
{
    registerMap["$zero"] = "00000";registerMap["$v0"]   = "00010";registerMap["$v1"]   = "00011";
    registerMap["$a0"]   = "00100";registerMap["$a1"]   = "00101";registerMap["$a2"]   = "00110";
    registerMap["$t0"]   = "01000";registerMap["$t1"]   = "01001";registerMap["$t2"]   = "01010";
    registerMap["$t3"]   = "01011";registerMap["$t4"]   = "01100";registerMap["$t5"]   = "01101";
    registerMap["$t6"]   = "01110";registerMap["$t7"]   = "01111";registerMap["$s0"]   = "10000";
    registerMap["$s1"]   = "10001";registerMap["$s2"]   = "10010";registerMap["$s3"]   = "10011";
    registerMap["$s4"]   = "10100";registerMap["$s5"]   = "10101";registerMap["$s6"]   = "10110";
    registerMap["$s7"]   = "10111";registerMap["$t8"]   = "11000";registerMap["$t9"]   = "11001";
    registerMap["$k0"]   = "11010";registerMap["$k1"]   = "11011";registerMap["$gp"]   = "11100";
    registerMap["$sp"]   = "11101";registerMap["$fp"]   = "11110";registerMap["$ra"]   = "11111";
}

/*! load the opcode of RIJ instructions by map
 *
 * @param opCodeMap
 */
void opCodeGenerator(map<string, string> &opCodeMap)
{
    //R-type
    opCodeMap["add"]  = "000000ssssstttttddddd00000100000";opCodeMap["addu"] = "000000ssssstttttddddd00000100001";
    opCodeMap["sub"]  = "000000ssssstttttddddd00000100010";opCodeMap["subu"] = "000000ssssstttttddddd00000100011";
    opCodeMap["and"]  = "000000ssssstttttddddd00000100100";opCodeMap["or"]   = "000000ssssstttttddddd00000100101";
    opCodeMap["xor"]  = "000000ssssstttttddddd00000100110";opCodeMap["nor"]  = "000000ssssstttttddddd00000100111";
    opCodeMap["div"]  = "000000dddddsssss0000000000011010";opCodeMap["divu"] = "000000dddddsssss0000000000011011";
    opCodeMap["slt"]  = "000000ssssstttttddddd00000101010";opCodeMap["sltu"] = "000000ssssstttttddddd00000101011";
    //add: shampt
    opCodeMap["sll"]  = "00000000000tttttdddddvvvvv000000";opCodeMap["srl"]  = "00000000000tttttdddddvvvvv000010";
    opCodeMap["jr"]    = "000000ddddd000000000000000001000";opCodeMap["mfhi"]  = "0000000000000000ddddd00000011100";
    opCodeMap["mflo"]  = "0000000000000000ddddd00000011010";opCodeMap["mult"]  = "000000dddddsssss0000000000011000";
    opCodeMap["multu"] = "000000dddddsssss0000000000011001";opCodeMap["sllv"]  = "000000ssssstttttddddd00000000100";
    opCodeMap["sra"]   = "00000000000tttttdddddvvvvv000011";opCodeMap["srav"]  = "000000ssssstttttddddd00000000111";
    opCodeMap["srlv"]  = "000000ssssstttttddddd00000000110";opCodeMap["teq"]   = "000000dddddsssss0000000000110100";
    opCodeMap["tge"]   = "000000dddddsssss0000000000110000";opCodeMap["tne"]   = "000000dddddsssss0000000000110110";
    opCodeMap["tgeu"]  = "000000dddddsssss0000000000110001";opCodeMap["tlt"]   = "00000dddddsssss0000000000110010";
    opCodeMap["tltu"]  = "000000dddddsssss0000000000110011";

    //I-type
    opCodeMap["addi"]   = "001000sssssdddddiiiiiiiiiiiiiiii";opCodeMap["addiu"]  = "001001sssssdddddiiiiiiiiiiiiiiii";
    opCodeMap["andi"]   = "001100sssssdddddiiiiiiiiiiiiiiii";opCodeMap["ori"]    = "001101sssssdddddiiiiiiiiiiiiiiii";
    opCodeMap["slti"]   = "001010sssssdddddiiiiiiiiiiiiiiii";opCodeMap["sltiu"]  = "001011sssssdddddiiiiiiiiiiiiiiii";
    opCodeMap["beq"]    = "000100dddddsssssffffffffffffffff";opCodeMap["bne"]    = "000101dddddsssssffffffffffffffff";
    opCodeMap["bgez"]   = "000001ddddd00001nnnnnnnnnnnnnnnn";opCodeMap["lb"]     = "100000sssssdddddxxxxxxxxxxxxxxxx";
    opCodeMap["lbu"]    = "10001sssssdddddxxxxxxxxxxxxxxxx";opCodeMap["lwl"]    = "10001sssssdddddxxxxxxxxxxxxxxxx";
    opCodeMap["lwr"]    = "100110sssssdddddxxxxxxxxxxxxxxxx";opCodeMap["lwcl"]   = "110001sssssdddddxxxxxxxxxxxxxxxx";
    opCodeMap["bltz"]   = "000001ddddd00000nnnnnnnnnnnnnnnn";opCodeMap["lw"]     = "100011sssssdddddxxxxxxxxxxxxxxxx";
    opCodeMap["sw"]     = "101011sssssdddddxxxxxxxxxxxxxxxx";opCodeMap["lui"]    = "00111100000dddddxxxxxxxxxxxxxxxx";
    opCodeMap["xori"]   = "001110sssssdddddiiiiiiiiiiiiiiii";opCodeMap["bgezal"] = "000001ddddd10001nnnnnnnnnnnnnnnn";
    opCodeMap["bgtz"]   = "000111ddddd00000nnnnnnnnnnnnnnnn";opCodeMap["blez"]   = "000110ddddd00000nnnnnnnnnnnnnnnn";
    opCodeMap["bltzal"] = "000001ddddd10000nnnnnnnnnnnnnnnn";opCodeMap["teqi"]   = "000001ddddd01100tttttttttttttttt";
    opCodeMap["tgei"]   = "000001ddddd01000tttttttttttttttt";opCodeMap["tgeiu"]  = "000001ddddd01001tttttttttttttttt";
    opCodeMap["tnei"]   = "000001ddddd01110tttttttttttttttt";opCodeMap["tlti"]   = "000001ddddd01010tttttttttttttttt";
    opCodeMap["tltiu"]  = "000001ddddd01011tttttttttttttttt";opCodeMap["lh"]     = "100001sssssdddddxxxxxxxxxxxxxxxx";
    opCodeMap["lhu"]    = "100101sssssdddddxxxxxxxxxxxxxxxx";opCodeMap["ll"]     = "110000sssssdddddxxxxxxxxxxxxxxxx";
    opCodeMap["sb"]     = "101000sssssdddddxxxxxxxxxxxxxxxx";opCodeMap["sh"]     = "101001sssssdddddxxxxxxxxxxxxxxxx";
    opCodeMap["swl"]    = "10101sssssdddddxxxxxxxxxxxxxxxx";opCodeMap["swr"]    = "10111sssssdddddxxxxxxxxxxxxxxxx";
    opCodeMap["mfhi"]   = "0000000000000000ddddd00000010000";opCodeMap["mflo"]   = "0000000000000000ddddd00000010010";
    opCodeMap["sc"]     = "111000sssssdddddxxxxxxxxxxxxxxxx";opCodeMap["mthi"]   = "000000ddddd000000000000000010001";
    opCodeMap["mtlo"]   = "000000ddddd000000000000000010011";opCodeMap["mfc0"]   = "01000000000dddddsssss00000000000";
    opCodeMap["mfcl"]   = "01000100000dddddsssss00000000000";opCodeMap["sdcl"]   = "111101sssssdddddxxxxxxxxxxxxxxxx";
    //J-type
    opCodeMap["j"]      = "000010jjjjjjjjjjjjjjjjjjjjjjjjjj";opCodeMap["jal"]    = "000011jjjjjjjjjjjjjjjjjjjjjjjjjj";
    opCodeMap["jalr"]   = "000000ddddd00000sssss00000001001";opCodeMap["jr"]     = "000000ddddd000000000000000001000";

}
