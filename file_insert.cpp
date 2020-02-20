#include "gdrive/gdrive.hpp"
#include <iostream>
#include <assert.h>
#include <vector>

using namespace GDRIVE;

int gdrive_out() {
    char* user_home = getenv("HOME");
    if (user_home == NULL) {
        fprintf(stderr, "No $HOME environment variable\n");
        exit(-1);
    }

    char default_gdrive_dir[512];
    strcpy(default_gdrive_dir, user_home);
    strcat(default_gdrive_dir, "/.gdrive/data");

    char* gdrive_dir = getenv("GDRIVE");
    if (gdrive_dir == NULL) {
        gdrive_dir = default_gdrive_dir;
    }
    
    FileStore fs(gdrive_dir);
    assert(fs.status() == SS_FULL);

    Credential cred(&fs);

    if (fs.get("refresh_token") == "") {
        std::string client_id = fs.get("client_id");
        std::string client_secret = fs.get("client_secret");

        OAuth oauth(client_id, client_secret);    
        std::cout << "Please go to this url using your browser, after you authorize this application, you will get a code from your browser" << std::endl
                  <<oauth.get_authorize_url() << std::endl;
        std::cout << "Please enter the code: ";
        std::string code;
        std::cin >> code;
        oauth.build_credential(code, cred);
    }

    Drive service(&cred);

    std::ifstream fin("lab_res.txt");
    FileContent fc(fin, "text/plain");
    GFile file;
    file.set_title("lab_res.txt");
    file.set_description("LAB result");
    file.set_mimeType("text/plain");
    service.files().Insert(&file, &fc).execute();
    fin.close();
}
