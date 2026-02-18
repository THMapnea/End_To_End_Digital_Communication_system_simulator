#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <time.h>



std::vector<int> bitGenerator(const int n_bits);
std::vector<int> addParityBits(const std::vector<int>& bits, const int parity_bits);
std::vector<int> HammingEncoder(const std::vector<int>& bte, const int data_bits, const int parity_bits);
std::vector<double> BPSKModulator(const std::vector<int>& btm, const double Eb);
void checkEnergy(const std::vector<double>& mb, int amplitude);

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




/*

	Function that generates n_bits in a random way

*/
std::vector<int> bitGenerator(const int n_bits){
	
	std::vector<int> bits(n_bits);
	
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
    
    std::vector<int> result(bits.size() + parity_bits);
    
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

std::vector<int> HammingEncoder(const std::vector<int>& bte, const int data_bits, const int parity_bits){
	
	std::vector<int> encoded_bits;
	
	
	
	
	
	
	return encoded_bits;
	
}




/*

	function that apply binary phase shifting key modulation based
	on a given energy Eb

*/

std::vector<double> BPSKModulator(const std::vector<int>& btm, const double Eb){
	
	double amplitude = std::sqrt(Eb);
	
	std::vector<double> symbols(btm.size());
	
	//apply bpsk
	for(const auto& b : btm){
		symbols.push_back(b == 0 ? -amplitude : amplitude);
	}
	
	return symbols;
}



/*

	function that controls if the signal energy has been preserved

*/

void checkEnergy(const std::vector<double>& mb, const int Eb){
	
	double amplitude = std::sqrt(Eb);
	
	int sum = 0;
	
	for(const auto& b : mb){
		sum += b * b;
	}
	
	int energy_found = sum / mb.size();
	
	if(energy_found == amplitude){
		std::cout<<"energy correctly conserved modulation successful\n";
	}else{
		std::cout<<"error in the modulation incorrect energy\n";
	}
}
