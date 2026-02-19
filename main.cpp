#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <chrono>
#include <thread>


//generator
std::vector<int> bitGenerator(const int n_bits);

//encoder and modulator
std::vector<int> addParityBits(const std::vector<int>& bits, const int parity_bits);
std::vector<int> hammingEncoder(const std::vector<int>& bte, const int data_bits, const int parity_bits);
std::vector<double> BPSKModulator(const std::vector<int>& btm, const double Eb);

//control
void checkEnergy(const std::vector<double>& mb, double amplitude);

//debug
void showBitString(const std::vector<int>& bits);
void showBPSKModulation(const std::vector<double>& symbols, int speed);



int main(){
	//seed setting
	srand((unsigned int)time(0));
	
	//bit to send
	std::vector<int> bts = bitGenerator(16);
	
	//apply Hamming channel coding
	std::vector<int> hamming_encoded_bits = hammingEncoder(bts, 4, 3);
	 
	//define the amplitude of the modulation
	double Eb = 1.3;
	
	//symbols for bpsk
	std::vector<double> symbols = BPSKModulator(hamming_encoded_bits, Eb);
	
	//check if the modulation was successful
	checkEnergy(symbols, Eb);
	
	//show the various bit strings
	std::cout<<"the original string is: ";showBitString(bts);
	std::cout<<"the Hamming encoded string is: ";showBitString(hamming_encoded_bits);
	
	//show the BPSK modulation
	showBPSKModulation(symbols, 50);

	return 0;
}




/*

	Function that generates n_bits in a random way

*/
std::vector<int> bitGenerator(const int n_bits){
	
	std::vector<int> bits;
	
	bits.reserve(n_bits);
	
	for(int i = 0; i < n_bits; i++){
		bits.push_back(rand() % 10 % 2);
	}
	
	return bits;
}



/*

	function that add the parity bit to a string of bits
	helper for the hamming code

*/
std::vector<int> addParityBits(const std::vector<int>& bits, const int parity_bits){
    
    std::vector<int> result(bits.size() + parity_bits); //dimension must be set for the for loop 
    
    result.reserve(bits.size() + parity_bits);
    
    int data_index = 0; //helper index variable
    
    //copies the data bits in the non even(bit) position
    //loop trough all the bits of the result
    for(int i = 1; i <= result.size(); i++){
	  	
        bool is_parity = false; //set the parity to false
        
        //loops and checks if the bit is in on of the parity bit position the loop
        //sets to true the boolean when we are in a parity bit position in the original array
        for(int j = 0; j < parity_bits; j++){
            if(i == pow(2, j)){
                is_parity = true;
                break;
            }
        
        }
        
        //if the bit is not a parity bit save the result and increase the helping index variable
        if(!is_parity){
        	//i -1 because i starts from 0 and the position from the original array of bits must be incremented
			// aside since bits is smaller than result as per Hamming
            result[i-1] = bits[data_index]; 
            data_index++;
        }
    }
    
    //compute the parity bits
    for(int j = 0; j < parity_bits; j++){
        int parity_position = pow(2, j) - 1;  //position 0-based
        int parity_bit = 0;
        
        // XOR of all the bits controlled by this parity bit
        for(int i = 0; i < result.size(); i++){
            // a bit is controlled if (i+1) & (1<<j) != 0 it's not the parity position itself
            if(((i+1) & (int)pow(2, j)) && (i != parity_position)){
                parity_bit ^= result[i];
            }
        }
        
        result[parity_position] = parity_bit;
    }
    
    return result;
}



/*

	function that applies an hamming encoding to the bits

*/

std::vector<int> hammingEncoder(const std::vector<int>& bte, const int data_bits, const int parity_bits){
	
	std::vector<int> encoded_bits;
	
	//helps to compute the reserve of the space
	int num_block = (bte.size() + data_bits - 1) / data_bits;
	
	encoded_bits.reserve(num_block * (data_bits + parity_bits));
	
	//!!!!need to fix if bte.size() is not a multiple of the data bits!!!!
	for(int i = 0; i < bte.size(); i+=data_bits){
		
		std::vector<int> helper;
		
		helper.reserve(data_bits);
		
		//build the block to encode that is the same size as the data bits
		for(int j = 0; j < data_bits; j++){
			helper.push_back(bte[i + j]);
		}
		
		//contains the new vector with the parity bits added built with copy constructor
		std::vector<int> helper_with_parity_bits(addParityBits(helper, parity_bits));
		
		encoded_bits.insert(encoded_bits.end(), helper_with_parity_bits.begin(), helper_with_parity_bits.end());
	}
	
	
	return encoded_bits;
	
}




/*

	function that apply binary phase shifting key modulation based
	on a given energy Eb

*/

std::vector<double> BPSKModulator(const std::vector<int>& btm, const double Eb){
	
	double amplitude = std::sqrt(Eb);
	
	std::vector<double> symbols;
	
	//apply bpsk
	for(const auto& b : btm){
		symbols.push_back(b == 0 ? -amplitude : amplitude);
	}
	
	return symbols;
}



/*

	function that controls if the signal energy has been preserved

*/

void checkEnergy(const std::vector<double>& mb, const double Eb){
	
	double amplitude = std::sqrt(Eb);
	
	double sum = 0;
	
	for(const auto& b : mb){
		sum += b * b;
	}
	
	double energy_found = sum / mb.size();
	
	double epsilon = 1e-10; //correct floating point division error
	
	if(energy_found - Eb < epsilon){
		std::cout<<"energy correctly conserved modulation successful\n";
	}else{
		std::cout<<"error in the modulation incorrect energy\n";
	}
}



/*

	function that shows a string of bits

*/

void showBitString(const std::vector<int>& bits){
	
	for(const auto& bit : bits){
		std::cout<<bit;
	}
	
	std::cout<<std::endl;
}



/*

	function the BPSK modulation

*/

void showBPSKModulation(const std::vector<double>& symbols, const int speed){
	std::cout<<"the symbols string is: \n";
	for(const auto& s : symbols){
		std::cout<<s;
		std::this_thread::sleep_for(std::chrono::milliseconds(speed));
		std::cout<<"\r";
		std::this_thread::sleep_for(std::chrono::milliseconds(speed));
	}
	
		
}
