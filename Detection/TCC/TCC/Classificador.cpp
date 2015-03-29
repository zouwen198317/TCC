#include "Classificador.h"


Classificador::Classificador()
{
	xml = "AdaBoost.xml";
}


Classificador::~Classificador()
{
}

void Classificador::LoadBoost()
{
	boost.load(xml.c_str());
}

void Classificador::Treina(vector<Mat> &caracteristica, vector<Mat> &naoCarracteristica)
{
	cout << "Comecando o treino" <<endl;
	vector<vector <float> > features;

	cout << "Atribuindo valor de carateristica" << endl;
	AtribuiValorAoVetor(features, caracteristica);
	
	cout << "Atribuindo valor de Nao carateristica" << endl;
	AtribuiValorAoVetor(features, naoCarracteristica);

	if (features.size() < 1)
		return;

	cout << "Tamanho vetor de features: " << features.size() << endl;
	cout << "Tamanho vetor de caracteristica: " << caracteristica.size() << endl;
	cout << "Tamanho vetor de naoCaracteristica: " << naoCarracteristica.size() << endl;

	Size dim(features[1].size(), features.size()); // (width, height)
	Mat trainData(dim, CV_32FC1, Scalar::all(0));
	Mat responses(Size(1, features.size()), CV_32SC1, Scalar::all(0));

	cout << "Carregando valores" << endl;
	// -------- - Carrega os vetores em cv::Mat para a fase de Treino--------------
	for (int i = 0; i < features.size(); i++)
	{
		
		if (i < caracteristica.size())
			responses.at<int>(i, 0) = 1;
		else
			responses.at<int>(i,0) = -1;
	}

	cout << "Terminou de carregar o vetor" << endl;
	cout << "Carregando o treino" << endl;
	for (int k = 0; k < trainData.rows; k++)
		for (int l = 0; l < trainData.cols; l++)
			trainData.at<float>(k, l) = features[k][l];

	// -----------------------------------------------------------------------------

	features.clear();

	cout << "Comecou treino de verdade" << endl;
	if (responses.depth() == CV_32SC1 && trainData.depth() == CV_32FC1)
	{
		// As amostras estao armazenadas em linhas ...
		cout << "Treinando..." << endl;
		boost.train(trainData, CV_ROW_SAMPLE, responses); //, Mat(), Mat(), Mat(), Mat(), BoostParams(CvBoost::REAL, 100, 0.95, 5, false, 0));
		cout << "Salvando treino" << endl;
		boost.save(xml.c_str());
	}
}

void Classificador::AtribuiValorAoVetor(vector<vector <float> > & vetor, vector<Mat>& local)
{
	for (unsigned i = 0; i < local.size(); i++)
	{
		Mat lbp = local.at(i);

		vector<float> temp;
		MatConstIterator_<float> it = lbp.begin<float>(), it_end = lbp.end<float>();

		for (; it != it_end; ++it)
			temp.push_back(*it);
		
		vetor.push_back(temp);

		temp.empty();
	}
}

float Classificador::Predicao(Mat image)
{
	Mat temp;

	MatConstIterator_<float> it = image.begin<float>(), it_end = image.end<float>();

	for (; it != it_end; ++it)
		temp.push_back(*it);

	return boost.predict(temp, Mat(), Range::all(), false, true);
}