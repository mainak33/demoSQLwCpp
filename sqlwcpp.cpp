#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <exception>
#include <iostream>
#include <string>

template<typename T>
void get_data(const std::string &prompt, T& value) {
    std::cout << prompt;
    std::cin >> value;
}

void insert_user(
    soci::session &sql,
    const std::string & first_name,
    const std::string & last_name,
    const std::string & email){
        //Insert data into users table
        sql 
        << "INSERT INTO users(first_name, last_name, email) VALUES(:fn, :ln, :e)",
        soci::use(first_name, "fn"), soci::use(last_name, "ln"), soci::use(email, "e");

        std::cout << "> Successfully inserted user." << std::endl << std::endl;
}

void delete_user(
    soci::session &sql,
    const int &id){
        //Delete data from users table
        sql 
        << "DELETE FROM users WHERE id = " << id;

        std::cout << "> Successfully deleted user." << std::endl << std::endl;
}

void display_users(soci::session& sql) {
    
    // Retrieve all rows from users table
    soci::rowset<soci::row> rs = (sql.prepare << "SELECT * FROM users");

    // Iterate through the result set
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) {
        const soci::row &r = *it;
        
        std::cout 
            << "ID: " << r.get<int>(0) << std::endl
            << "First Name: " << r.get<std::string>(1) << std::endl
            << "Last Name: " << r.get<std::string>(2) << std::endl
            << "Email: " << r.get<std::string>(3) << std::endl
            << "Active: " << r.get<int>(4) 
            << std::endl 
            << std::endl;
    }
}

struct Option{
    
    int value = 0;
    
    enum class Description{
        LIST,
        ADD,
        DELETE,
        QUIT,
        INVALID
    };
    
    auto get_description() -> Option::Description{
        Option::Description optionDescription = Option::Description::INVALID;

        if     (1 == value) optionDescription = Option::Description::LIST;
        else if(2 == value) optionDescription = Option::Description::ADD;
        else if(3 == value) optionDescription = Option::Description::DELETE;
        else if(4 == value) optionDescription = Option::Description::QUIT;

        return optionDescription;
    };
};

int main() {

    try {

        // Connect to MySQL database
        soci::session sql(soci::mysql, "db=test_db user=test_dev1 password=Secure123!!");

        // Initializations
        std::string first_name, last_name, email;
        bool quit = false;
        Option option;        
        using OptionDescription = Option::Description;

        std::string optionsMessage = 
            "Options: \n"
            "\t 1 : List all users \n"
            "\t 2 : Add an entry to users \n"
            "\t 3 : Delete an entry from users \n"
            "\t 4 : Exit  \n\n"
            "> Please select an option: ";
        
        while (!quit){
            // Get option
            option.value = -1;
            get_data(optionsMessage, option.value);
            
            // Excercise option
            if (OptionDescription::LIST == option.get_description()){
                // Retrieve all rows from users table and output data
                display_users(sql);
            }else if (OptionDescription::ADD == option.get_description()){
                get_data("> Enter first name: ", first_name);
                get_data("> Enter last name: ", last_name);
                get_data("> Enter email address: ", email);
                
                // Insert a new row into users table
                insert_user(sql, first_name, last_name, email);

                // Retrieve all rows from users table and output data
                display_users(sql);
            }else if (OptionDescription::DELETE == option.get_description()){
                int idToDelete = -1;
                get_data("> Enter id of user to be deleted: ", idToDelete);
                if (idToDelete >= 0)
                    delete_user(sql,idToDelete);
            }else if (OptionDescription::QUIT == option.get_description()){
                quit = true;
            }else{
                std::cout << "> Invalid option selected." << std::endl << std::endl;
            }
        }
        
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl; 
    }

    return 0;
}