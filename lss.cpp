//  lss.cpp

#include <iostream>
#include <map>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

using namespace std;

struct Shot {
    string name;
    string extension;
    long padding;
    vector<int> frames;
};

int main(int argc, const char * argv[]) {
    // create a dictionary of all shots {'shot': {'name': 'shot', 'ext': '.jpeg', 'padding': 4, 'frames': [1,2,3,10,11,12]}}
    std::map<string, Shot> all_shots;
    // keep all the invalid files in extras
    vector<string> extras;
    
    boost::regex expr{"^(.*)((?<![a-zA-Z0-9])\\d+)(.*)$"};
    boost::smatch reg_match;
    
    // get the first argument or currenct folder
    boost::filesystem::path root(argc>1 ? argv[1]: ".");
    
    if (boost::filesystem::is_directory(root)){
        for (auto&& x : boost::filesystem::directory_iterator(root)){
            string filename = x.path().filename().string();
    
            // if valid match then save the data in all_shots;
            if (boost::regex_match(filename, reg_match, expr)){
                string name = reg_match[1];
                long padding = reg_match[2].length();
                string extension = reg_match[3];
                int frame;
                stringstream frameStream;
                
                // converting from char to long
                frameStream << reg_match[2];
                frameStream >> frame;
                
                // check if shot already doesn't exist;
                if (all_shots.find(name) == all_shots.end()){
                    Shot shot{name, extension, padding};
                    shot.frames.push_back(frame);
                    all_shots[name] = shot;
                } else {
                    all_shots.at(name).frames.push_back(frame);
                }
            } else {
                // put all the invalid files.
                extras.push_back(filename);
            }
        }
        
        /*
            iterate over all shots, at this point the frames are like [1,2,3,4,5,10,11,12,13] and
            I need to make it to [[1,2,3,4,5], [10,11,12,13]]
         */
        for (auto it=all_shots.begin(); it != all_shots.end(); ++it){
            Shot *shot = &it->second;
            std::sort(shot->frames.begin(), shot->frames.end());
            
            vector<vector<int>> all_frames;
            vector<int> temp_frames;
            int last_frame = 0;
        
            for (auto itt=shot->frames.begin(); itt!=shot->frames.end(); ++itt){
                if (temp_frames.size() > 0 && *itt != last_frame+1){
                    all_frames.push_back(temp_frames);
                    temp_frames.clear();
                };
                temp_frames.push_back(*itt);
                last_frame = *itt;
            }
            all_frames.push_back(temp_frames);
            
            for (vector<vector<int>>::iterator frames=all_frames.begin(); frames!=all_frames.end(); ++frames){
                // get the min and max frame numbers
                auto minmax = std::minmax_element(frames->begin(), frames->end());
                
                // print everything
                printf("%s%%0%ldd%s\t\t(%ld)\t%d-%d\n", shot->name.c_str(), shot->padding, shot->extension.c_str(), frames->size(),
                       *minmax.first, *minmax.second);
            }
        }
        
        // print the rest of the files
        for (auto ext=extras.begin(); ext!= extras.end(); ++ext){
            cout << *ext << endl;
        }
    }
}
