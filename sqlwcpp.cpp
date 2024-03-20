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

struct Options{
    enum class E_options{
        LIST,
        ADD,
        DELETE,
        QUIT,
        INVALID
    };
    auto operator()(int optionIn) -> Options::E_options{
        Options::E_options optionDescription = Options::E_options::INVALID;

        if     (1 == optionIn) optionDescription = Options::E_options::LIST;
        else if(2 == optionIn) optionDescription = Options::E_options::ADD;
        else if(3 == optionIn) optionDescription = Options::E_options::DELETE;
        else if(4 == optionIn) optionDescription = Options::E_options::QUIT;

        return optionDescription;
    };
};

int main() {

    try {

        // Connect to MySQL database
        soci::session sql(soci::mysql, "db=test_db user=test_dev1 password=Secure123!!");

        // Initializations
        std::string first_name, last_name, email;
        int optionValue = -1;
        bool quit = false;
        Options options;        
        using OptionDescriptions = Options::E_options;
        OptionDescriptions optionDescription = OptionDescriptions::INVALID;

        std::string optionsMessage = 
            "Options: \n"
            "\t 1 : List all users \n"
            "\t 2 : Add an entry to users \n"
            "\t 3 : Delete an entry from users \n"
            "\t 4 : Exit  \n\n"
            "> Please select an option: ";
        
        while (!quit){
            // Get option
            optionValue = -1;
            get_data(optionsMessage, optionValue);
            optionDescription = options(optionValue);
            
            // Excercise option
            if (OptionDescriptions::LIST == optionDescription){
                // Retrieve all rows from users table and output data
                display_users(sql);
            }else if (OptionDescriptions::ADD == optionDescription){
                get_data("> Enter first name: ", first_name);
                get_data("> Enter last name: ", last_name);
                get_data("> Enter email address: ", email);
                
                // Insert a new row into users table
                insert_user(sql, first_name, last_name, email);

                // Retrieve all rows from users table and output data
                display_users(sql);
            }else if (OptionDescriptions::DELETE == optionDescription){
                int idToDelete = -1;
                get_data("> Enter id of user to be deleted: ", idToDelete);
                if (idToDelete >= 0)
                    delete_user(sql,idToDelete);
            }else if (OptionDescriptions::QUIT == optionDescription){
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