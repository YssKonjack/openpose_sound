#include <iostream>
#include <string>


int main()
{
    std::string input;
    int dataCount = 0;
    int personCount = 0;
    std::string dataToPrint = "";

    while(input != "quit"){
        std::cin >> input;

        if("reset" == input){
            std::cout << "" << std::endl;
            dataCount = 0;
            personCount = 0;
        }else{
            if ("Person" == input){
                std::cout << "Person: " << personCount << " (x, y, score):" << std::endl;
                personCount += 1;
            }else {
                dataToPrint += input + " ";
                if(2 == dataCount%3){
                    std::cout << dataToPrint << std::endl;
                    dataToPrint = "";
                }
                dataCount++;
            }
        }
    }
}
