#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

vector<string> split_at_semicolon(const string& row)
{
	vector<string> res;
	istringstream buf(row);
	string s;
	while (getline(buf, s, ';')) res.push_back(s);
	return res;
}



int main_off() {

	const int I = 3;
	const int J = 3;
	const int K = 31;

	// ������ ������ - �������, ������ ������ - ��������, ������ ������ - �����. 
	double plan[I][J][K];	 // ������ �������� ������ �� ���������/���������
	double planBias[I][J][K]; // ������ �������� ������ �� ���������/��������� � ������ ��������
	double fact0[I][J][K];	 // ������ �������� �������� ������ �� ���������/���������
	double fact1[I][J][K];	 // ������ ��� �������� �������� ������ �� ���������/��������� �� 1-�� ����� �������������
	double fact2[I][J][K];	 // ������ ��� �������� �������� ������ �� ���������/��������� �� 2-�� ����� �������������

	for (int i = 0; i < I; i++) {
		for (int j = 0; j < J; j++) {
			for (int k = 0; k < K; k++) {
				plan[i][j][k] = 0;
				planBias[i][j][k] = 0;
				fact0[i][j][k] = 0;
				fact1[i][j][k] = 0;
				fact2[i][j][k] = 0;
			}
		}
	}

	string row;

	// �������� ����������� ����� � ������� ����� � ������� ������
	std::locale lcl;
	const auto & facet = std::use_facet<std::numpunct<char>>(lcl);
	char decimal_point = facet.decimal_point();

	ifstream wInv("Input2.csv");
	if (!wInv.bad()) {
		int lineNum = 0;
		while (getline(wInv, row)) {
			vector<string> values = split_at_semicolon(row);
			if (lineNum > 0) {
				int ContractNum = 0;
				istringstream issCN(values[0]);
				issCN >> ContractNum;

				int GazPointNum = 0;
				istringstream issGPN(values[1]);
				issGPN >> GazPointNum;

				for (int k = 3; k < values.size(); k++) {
					double ftmp = 0.0;
					replace(values[k].begin(), values[k].end(), ',', decimal_point);
					istringstream istmp;
					istmp.str(values[k]);
					if (values[2] == "P") {
						istmp >> plan[ContractNum][GazPointNum][k - 2];
					}
					else if (values[2] == "F") {
						istmp >> fact0[ContractNum][GazPointNum][k - 2];
					}
				}
			}
			lineNum++;
		}
		wInv.close();
	}
	else {
		cout << "Can't open input data file" << endl;
	}


	// ���������� ��������� ��������
	// ������ ������ - �������, ������ ������ - ��������, ������ ������ - �����. 
	// ������� �������� � ������� - ����� ����� �������������� �� ���������, ��������� � �.�.

	for (int i = 1; i < I; i++) {
		for (int j = 1; j < J; j++) {
			for (int k = 1; k < K; k++) {
				plan[i][0][0] += plan[i][j][k];		// ��������� ���� �� ������� �������� �� �����
				plan[i][j][0] += plan[i][j][k];		// ��������� ���� �� ������ ����� ������� �������� �� �����
				plan[i][0][k] += plan[i][j][k];		// ��������� ���� �� �������� �� �����
				plan[0][j][k] += plan[i][j][k];		// ��������� ���� �� ����� �� �����
				plan[0][0][k] += plan[i][j][k];		// ���� ���� �� �����

															// ������ ����� ������� 1.1 ?!?!? (BIAS) TODO	  (technical debt)
				planBias[i][j][k] = floor(plan[i][j][k] * 1.1 * 1000 + 0.5) / 1000;  // �������� ���� �� 3-�� ����� ����� �������
				//planBias[i][j][0] += planBias[i][j][k];		// ��������� ���� �� ������ ����� ������� �������� �� ����� � ������ �������� 1.1
				planBias[i][j][0] = floor(plan[i][j][0] * 1.1 * 1000 + 0.5) / 1000;
				//planBias[i][0][k] += planBias[i][j][k];		// ��������� ���� �� �������� �� ����� � ������ �������� 1.1
				planBias[i][0][k] = floor(plan[i][0][k] * 1.1 * 1000 + 0.5) / 1000;
				//planBias[0][j][k] += planBias[i][j][k];		// ��������� ���� �� ����� �� ����� � ������ �������� 1.1
				planBias[0][j][k] = floor(plan[0][j][k] * 1.1 * 1000 + 0.5) / 1000;
				//planBias[0][0][k] += planBias[i][j][k];		// ���� ���� �� ����� � ������ �������� 1.1
				planBias[0][0][k] = floor(plan[0][0][k] * 1.1 * 1000 + 0.5) / 1000;

				fact0[i][j][0] += fact0[i][j][k];	// ����� �� ����� �� �����
				fact0[i][0][k] += fact0[i][j][k];	// ����� �� �������� �� �����
				fact0[0][j][k] += fact0[i][j][k];	// ����� �� ����� �� �����
				fact0[0][0][k] += fact0[i][j][k];	// ���� ���� �� �����
			}
		}
	}

/*
		for (int k = 0; k < K; k++) {
			cout << plan[0][0][k] << "\t" << plan[0][1][k] << "\t" << plan[0][2][k] << "\t";
			cout << plan[1][0][k] << "\t" << plan[1][1][k] << "\t" << plan[1][2][k] << "\t";
			cout << plan[2][0][k] << "\t" << plan[2][1][k] << "\t" << plan[2][2][k] << "\n";
		}

		for (int k = 0; k < K; k++) {
			cout << fact0[0][0][k] << "\t" << fact0[0][1][k] << "\t" << fact0[0][2][k] << "\t";
			cout << fact0[1][0][k] << "\t" << fact0[1][1][k] << "\t" << fact0[1][2][k] << "\t";
			cout << fact0[2][0][k] << "\t" << fact0[2][1][k] << "\t" << fact0[2][2][k] << "\n";
		}
	
		for (int k = 0; k < K; k++) {
			cout << planBias[0][0][k] << "\t" << planBias[0][1][k] << "\t" << planBias[0][2][k] << "\t";
			cout << planBias[1][0][k] << "\t" << planBias[1][1][k] << "\t" << planBias[1][2][k] << "\t";
			cout << planBias[2][0][k] << "\t" << planBias[2][1][k] << "\t" << planBias[2][2][k] << "\n";
		}
 */

	// ��������� �������� ����� �� �������� �� ������ ����� �������������
	for (int k = 1; k < K; k++) {

		if (fact0[0][0][k] > planBias[0][0][k]) {
			// ��������� �� �. 2.1.
			fact1[2][0][k] = planBias[2][0][k];						// ������� �� ������ � ������ �������.
			fact1[1][0][k] = fact0[0][0][k] - fact1[2][0][k];		// TODO: ������������ ����� ���������, ������������� ����� �� ��������� ���������� �������� (technical debt)
		}
		else {
			// ��������� �� �. 2.2.
			double tmp[] = { fact0[0][0][k], planBias[1][0][k] };
			fact1[1][0][k] = *min_element(tmp, tmp + 2);
			fact1[2][0][k] = fact0[0][0][k] - fact1[1][0][k];
		}

		//cout << "Day " << k << " : " << fact1[1][0][k] << " : " << fact1[2][0][k] << endl;
	}

	// ��������� ����� ��������� ��������� ���� �� ��������� ����� ������� ����� �������������
	for (int i = 1; i < I; i++) {
		for (int k = 1; k < K; k++) {
			fact1[i][0][0] += fact1[i][0][k];
		}
		//cout << "Contract = " << i << " fact1 = " << fact1[i][0][0] << endl;
	}

	// ��������� ����� ���������� ������ ������������
	// TODO: ������� ������ ������ ������� ����� �������������	(technical debt)

	// ������ �� ��������� 3.2
	//
	// =����(Y6<(F6+H6);Y6+���(������((F6+H6)*1,1;3)-Y6;($F$36+$H$36)-$Y$36;$Z$36-($B$36+$D$36);Z6);Y6)
	// , ��� k=1
	// =����(Y7<(F7+H7);Y7+���(������((F7+H7)*1,1;3)-Y7;($F$36+$H$36)-AQ6;AS6-($B$36+$D$36);Z7);Y7)
	// , ��� k>1
	//
	// h0 = Y7<(F7+H7) = fact1[2][0][k] < plan[2][0][k]
	// h1 = ������((F7+H7)*1,1;3)-Y7 = planBias[2][0][k] - fact1[2][0][k]
	// h2 = ($F$36+$H$36)-AQ6 = plan[2][0][0] - fact2[2][0][0]
	// h3 = AS6-($B$36+$D$36) = fact2[1][0][0] - plan[1][0][0]
	// h4 = Z7 = fact1[1][0][k]
	// h5 = Y7 = fact1[2][0][k]
	// h6 = Y7+���(������((F7+H7)*1,1;3)-Y7;($F$36+$H$36)-AQ6;AS6-($B$36+$D$36);Z7) = h5 + min(h1, h2, h3, h4)
	// fact2[2][0][k] = (h0 ?: h6, h5)

	fact2[1][0][0] = fact1[1][0][0];
	fact2[2][0][0] = fact1[2][0][0];

	for (int k = 1; k < K; k++) {
		if (fact1[2][0][k] < plan[2][0][k]) {
			double h1 = planBias[2][0][k] - fact1[2][0][k];
			double h2 = plan[2][0][0] - fact2[2][0][0];
			double h3 = fact2[1][0][0] - plan[1][0][0];
			double h4 = fact1[1][0][k];
			double h5 = fact1[2][0][k];
			double tmp1[] = { h1, h2, h3, h4 };
			double h6 = h5 + *min_element(tmp1, tmp1 + 4);
			fact2[2][0][k] = h6;
		}
		else {
			fact2[2][0][k] = fact1[2][0][k];
		}

		fact2[2][0][0] += fact2[2][0][k] - fact1[2][0][k];
		fact2[1][0][0] -= fact2[2][0][k] - fact1[2][0][k];
		fact2[1][0][k] = fact1[1][0][k] - (fact2[2][0][k] - fact1[2][0][k]);;

		//cout << "fact2[2][0][" << k << "] = " << fact2[2][0][k] << "\tfact2[1][0][" << k << "] = " << fact2[1][0][k] << endl;
		//cout << "fact2[2][0][0] = " << fact2[2][0][0] << endl;
		//cout << "fact2[1][0][0] = " << fact2[1][0][0] << endl;
	}


	/*
	// ������ ������ �� ��������� 3.3
	//
	// fact2[2][0][k] = Y6-���(Y6;$Y$36;($D$36+$B$36)-$Z$36;����(���((Y6+Z6);������((B6+D6)*1,1;3))-Z6;0))
	// , ��� k=1
	// fact2[2][0][k] = Y7-���(Y7;AQ6;($D$36+$B$36)-AS6;����(���((Y7+Z7);������((B7+D7)*1,1;3))-Z7;0))
	// ? ��� k>1
	//
	// h1 = ������((B7+D7)*1,1;3) = planBias[1][0][k]
	// h2 = (Y7+Z7) = fact1[1][0][k] + fact1[2][0][k]
	// h3 = ���((Y7+Z7);������((B7+D7)*1,1;3))-Z7 = min(h2; h1) - fact1[1][0][k]
	// h4 = ����(���((Y7+Z7);������((B7+D7)*1,1;3))-Z7;0) = max(h3; 0)
	// h5 = ($D$36+$B$36)-AS6 = plan[1][0][0] - fact2[1][0][0]
	// h6 = AQ6 = fact2[2][0][0]
	// h7 = Y7 = fact1[2][0][k]
	// fact2[2][0][k] = h7 - min(h7, h6, h5, h4)

	fact2[1][0][0] = fact1[1][0][0];
	fact2[2][0][0] = fact1[2][0][0];

	for (int k = 1; k < K; k++) {
		double h1 = planBias[1][0][k];
		double h2 = fact1[1][0][k] + fact1[2][0][k];
		double tmp1[] = { h2, h1 };
		double h3 = *min_element(tmp1, tmp1 + 2) - fact1[1][0][k];
		double tmp2[] = { h3, 0 };
		double h4 = *max_element(tmp2, tmp2 + 2);
		double h5 = plan[1][0][0] - fact2[1][0][0];
		double h6 = fact2[2][0][0];
		double h7 = fact1[2][0][k];
		//cout << "h1=" << h1 << " h2=" << h2 << " h3=" << h3 << " h4=" << h4 << " h5=" << h5 << " h6=" << h6 << " h7=" << h7 << endl;
		double tmp3[] = { h7,h6,h5,h4 };
		fact2[2][0][k] = h7 - *min_element(tmp3, tmp3 + 4);

		// =$Y$36-(Y6-AP6), ��� k=1
		// =AQ6-(Y7-AP7), ��� k>1
		fact2[2][0][0] -= fact2[2][0][k] - fact1[2][0][k];
		// =$Z$36+(Y6-AP6), ��� k=1
		// =AS6+(Y7-AP7), ��� k>1
		fact2[1][0][0] += fact2[2][0][k] - fact1[2][0][k];
		// =Z6+(Y6-AP6)
		fact2[1][0][k] = fact1[1][0][k] + (fact2[2][0][k] - fact1[2][0][k]);

		//cout << "fact2[2][0][" << k << "] = " << fact2[2][0][k] << "\tfact2[1][0][" << k << "] = " << fact2[1][0][k] << endl;
		//cout << "fact2[2][0][0] = " << fact2[2][0][0] << endl;
		//cout << "fact2[1][0][0] = " << fact2[1][0][0] << endl;
	}
	 */

	 // �������� ����������� ���� �� ���������
	for (int i = 1; i < I; i++) {
		for (int j = 1; j < J; j++) {
			for (int k = 1; k < K; k++) {
				if (fact0[0][0][k] != 0) fact2[i][j][k] = fact2[i][0][k] * fact0[0][j][k] / fact0[0][0][k];
				cout << "fact2[" << i << "][" << j << "][" << k << "] = " << fact2[i][j][k] << endl;
			}
		}
	}


	system("pause");
	return 0;
}