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
            if(time_sick <=0){
                state = "vaccinated";
                infected = false;
                vaccinated = true;
            }
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
        int caught = rand()&100;
        int safe;

        for(Person p: People){
            if(p.get_state() == "vaccinated"){
                safe++;
            }
        }

        if(safe > 8000){
            if(caught < 5){
                return true;
            } else{
                return false;
            }
        } else{
            if(caught < 10){
                return true;
            } else{
                return false;
            }
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
            index++;
        }
    }

    void get_statistics(int &sick, int &vaccinated, int &well){
        sick = 0;
        vaccinated = 0;
        well = 0;

        for (Person p: People){
            if(p.get_state() == "infected"){
                sick++;
            }
            else if(p.get_state() == "vaccinated"){
                vaccinated++;
            }
            else{
                well++;
            }
        }
    }
};

int main(){

    //srand(time(NULL));

    int population_size = 10000;
    int time_sick = 5;
    int daily_interactions = 10;

    int num_sick;
    int num_vaccinated;
    int num_well;

    Population COVID_Pop(population_size, time_sick);

    int day = 1;

    while(num_sick > 1){
        COVID_Pop.get_statistics(num_sick, num_vaccinated, num_well);
        cout << day << ", "<< num_sick<<", "<< num_vaccinated<<endl;

        day++;
        COVID_Pop.update(daily_interactions, time_sick);
    }
    cout<<"done"<<endl;
    return 0;

}
