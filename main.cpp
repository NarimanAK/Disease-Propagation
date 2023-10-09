#include <iostream>
#include <vector>
#include <string>
#include <time.h>


using std::endl;
using std::cout;
using std::string;
using std::vector;

class Person{

private:
    int time_sick;
    string state = "well";
    bool infected = false;
    bool vaccinated = false;
    int age;
public:
    Person(){
    }

    //Person(int x){
      //  age = x;
    //}

    void infect(int days){
        if(!infected || !vaccinated){
            state = "infected";
            infected = true;
            time_sick = days;
        }
    }

    void update(){
        if(infected){
            time_sick = time_sick - 1;
            
        }
    }

    void vaccinate(){
        
        state = "vaccinated";
        infected = false;
        vaccinated = true;
    
    }

    string get_state(){
        return state;
    }

    int get_time_sick(){
        return time_sick;
    }
};

class Population{

private:
    vector<Person> People;
    int population_size;

public:
    Population(){
        srand(time(NULL));
    }

    Population(int size, int time_sick){
        srand(time(NULL));
        population_size = size;
        Person p_zero;
        p_zero.infect(time_sick);
        People.push_back(p_zero);

        for(int i = 1; i < size; i++){
            Person p;
            People.push_back(p);
        }
    }

    bool luck(){
        int caught = rand()%100;
        int safe = 0;
        
        for(Person p: People){
            if(p.get_state() == "vaccinated"){
                safe++;
            }
        }
        if(caught < 10){
            return true;
        } else{
            return false;
        }  
    }

    void update(int number_of_interactions, int time_sick){
        int index = 0;
        int roll =0;
        int days = time_sick + 1;
        bool catch_infection;

        for(Person &p: People){
            p.update();
            if(p.get_state() == "well"){
                for(int i = 0; i < number_of_interactions; i++){
                    roll = rand()%population_size;
                    bool catch_infection = luck();

                    if(People[roll].get_state() == "infected"){
                        if(catch_infection){
                            p.infect(days);
                        }
                    }
                }
            }
            else if(p.get_state() == "infected"){
                if(p.get_time_sick() > 1){
                    for(int i = 0; i < number_of_interactions; i++){
                        roll = rand()%population_size;
                        bool catch_infection = luck();

                        if(People[roll].get_state() == "well"){
                            if(catch_infection){
                                People[roll].infect(days);
                            }
                        }
                    }
                }
                else if(p.get_time_sick() <= 0){
                    p.vaccinate();
                }              
            index++;
            }
        }
    }
    void get_statistics(int &sick, int &vaccinated, int &well) {
        sick = 0;
        vaccinated = 0;
        well = 0;

        for (Person p : People) {
            if (p.get_state() == "infected") {
                sick++;
            } else if (p.get_state() == "vaccinated") {
                vaccinated++;
            } else {
                well++;
            }
        }
    }   
};

int main(){

    srand(time(NULL));

    int population_size = 1000;
    int time_sick = 5;
    int daily_interactions = 10;

    int num_sick = 1;
    int num_vaccinated = 0;
    int num_well= 0;

    Population COVID_Pop(population_size, time_sick);

    int day = 1;

    while (num_sick > 0) {
        COVID_Pop.get_statistics(num_sick, num_vaccinated, num_well);
        cout << "Day " << day << ": " << num_sick << " sick individuals" << endl;
        day++;
        COVID_Pop.update(daily_interactions, time_sick);
    }

    cout<<"done"<<endl;
    return 0;
}
