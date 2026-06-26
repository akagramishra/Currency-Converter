#include<cpr/cpr.h>
#include<nlohmann/json.hpp>
#include<iostream>
#include<algorithm>
#include<string>
#include<cctype>
#include<cstdlib>

using json = nlohmann::json;

int main(){
    cpr::Session session_currency;
    cpr::Session session_name;
    const char* APIk = std::getenv("API_KEY");
    if(!APIk){ return 1;}
    std::string key(APIk);
    session_name.SetUrl(cpr::Url{"https://api.freecurrencyapi.com/v1/currencies?apikey=" + key});
    session_currency.SetUrl(cpr::Url{"https://api.freecurrencyapi.com/v1/latest?apikey="+key});
    cpr::Response r_name = session_name.Get();
    cpr::Response r_currency = session_currency.Get();
    
    json name_data = json::parse(r_name.text);
    
    json data = json::parse(r_currency.text);
    

    while(true){
        std::string fromCurrency, toCurrency;
        double amount;
        std::cout<<"Enter the currency you want to convert from ";
        std::cin>>fromCurrency;

        std::transform(fromCurrency.begin(), fromCurrency.end(), fromCurrency.begin(), [](unsigned char c){ return std::toupper(c); });

        if(toCurrency.compare("EXIT") == 0 || fromCurrency.compare("EXIT") == 0){
    
            std::cout<<"Exiting the program..."<<'\n'<<"Thanks for Using the Currency Converter!"<<std::endl;
            break;
        }

        std::cout<<"Enter the amount you want to convert ";
        std::cin>>amount;

        
        std::cout<<"Enter the currency you want to convert to ";
        std::cin>>toCurrency;

        std::transform(toCurrency.begin(), toCurrency.end(), toCurrency.begin(), [](unsigned char c){ return std::toupper(c); });

        if (!data["data"].contains(fromCurrency) || !data["data"].contains(toCurrency)) {
        std::cout << "Unknown currency code.\n";
        continue;    
        }
        double toRate = static_cast<double>(data["data"][toCurrency])/ static_cast<double>(data["data"][fromCurrency]);

        
        std::cout<< "The converted amount is: "<<name_data["data"][toCurrency]["symbol_native"].get<std::string>()<<" "<<toRate*amount<<std::endl;

    
    }
    
}
