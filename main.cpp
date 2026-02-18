#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <time.h>



std::vector<int> bitGenerator(int n_bits);
std::vector<double> BPSKModulator(std::vector<int> btm, double Eb);
void checkEnergy(std::vector<double> mb, int amplitude);

int main(){
	//seed setting
	srand((unsigned int)time(0));
	
	//bit to send
	std::vector<int> bts = bitGenerator(16);
	
	//apply Hamming channel coding
	 
	
	//define the amplitude of the modulation
	double Eb = 1.1;
	
	//symbols for bpsk
	std::vector<double> symbols = BPSKModulator(bts, Eb);
	
	//check if the modulation was successful
	checkEnergy(symbols, Eb);
	
		
	return 0;
}

std::vector<int> bitGenerator(int n_bits){
	
	std::vector<int> bits;
	
	for(int i = 0; i < n_bits; i++){
		bits.push_back(rand() % 10 % 2);
	}
	
	return bits;
}

std::vector<double> BPSKModulator(std::vector<int> btm, double Eb){
	
	double amplitude = std::sqrt(Eb);
	
	std::vector<double> symbols;
	
	//apply bpsk
	for(int i = 0; i < btm.size(); i++){
		symbols.push_back(btm[i] == 0 ? -amplitude : amplitude);
	}
	
	return symbols;
}

void checkEnergy(std::vector<double> mb, int Eb){
	
	double amplitude = std::sqrt(Eb);
	
	int sum = 0;
	
	for(int i = 0; i < mb.size(); i++){
		sum += mb[i] * mb[i];
	}
	
	int energy_found = sum / mb.size();
	
	if(energy_found == amplitude){
		std::cout<<"energy correctly conserved modulation successful\n";
	}else{
		std::cout<<"error in the modulation incorrect energy\n";
	}
}
