#include <iostream>
#include <fstream>
#include <sstream>

#include "Comment.h"

class TreeNode {
public:
    TreeNode() : left(nullptr), right(nullptr) {}
    TreeNode(const Comment& init) : value(init), left(nullptr), right(nullptr) {}
    Comment value;
    TreeNode* left;
    TreeNode* right;
};

void addNode(TreeNode*& parent, Comment& value, bool& found) {
    // if node is empty and its in matching parent
    if (parent==nullptr and found) {
        parent = new TreeNode(value);
    }
    // if not a reply then add a node to the left
    else if (value.getParent()=="" or found) {
        found = true;
        addNode(parent->left, value, found);
    }// if found matching comment, add reply to the right
    else if (value.getParent()==parent->value.getId() or found) {
        found = true;
        int rep= parent->value.getReplies()+1;
        parent->value.setReplies(rep);
        addNode(parent->right, value, found);
    }
    else {
        // keep searching
        if (parent->left != nullptr) {
            addNode(parent->left, value, found);
        }
        if (parent->right != nullptr) {
            addNode(parent->right, value, found);
        }
    }
}


void likeComment(TreeNode*& root, std::string& id) {
    // find comment and changes likes
    if (root==nullptr) {
        return;
    }
    if (root->value.getId() == id) {
        int like = root->value.getLikes() + 1;
        root->value.setLikes(like);
    }
    likeComment(root->right, id);
    likeComment(root->left, id);
}

void deleteTree(TreeNode*& root) {
    if (root == nullptr) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root; // Delete the current node
}

void deleteComment(TreeNode*& root, std::string& id) {
    // finds comment and deletes whole chain
    if (root==nullptr) {
        return;
    }
    if (root->value.getId() == id) {
        deleteTree(root);
        root = nullptr;
        return;
    }
    deleteComment(root->right, id);
    deleteComment(root->left, id);
}

void printTree(TreeNode*& root, std::ofstream& output, int& indent) {
    if (root == nullptr) {
        return;
    }
    // output
    Comment com = root->value;
    std::string space;
    for (int i=0; i>indent; i++) {
        space += "    ";
    }
    output << space << com.getAuthor() << " " << com.getDate() << std::endl;
    output << space << com.getText() << std::endl;
    output << space << "\U0001F44D " << com.getLikes() << std::endl;
    if (root->right != nullptr) {
        std::string reply = " reply";
        if (com.getReplies()>1) {
            reply = " replies";
        }
        output << space << com.getReplies() << reply << std::endl;
    }

    // goes to other comments
    int newind = indent+1;
    printTree(root->right, output, newind);
    printTree(root->left, output, indent);
    if (indent>0) {

    }

}

void displayComment(TreeNode*& root, std::string& id, std::ofstream& output) {
    // finds comment to display
    if (root==nullptr) {
        return;
    }
    if (root->value.getId() == id) {
        int indent = 0;
        // prints tree from that comment as the root
        Comment com = root->value;
        std::string space;
        for (int i=0; i>indent; i++) {
            space += "    ";
        }
        output << space << com.getAuthor() << " " << com.getDate() << std::endl;
        output << space << com.getText() << std::endl;
        output << space << "\U0001F44D " << com.getLikes() << std::endl;
        if (root->right != nullptr) {
            std::string reply = " reply";
            if (com.getReplies()>1) {
                reply = " replies";
            }
            output << space << com.getReplies() << reply << std::endl;
        }

        printTree(root->right, output, indent);
        return;
    }
    displayComment(root->right, id, output);
    displayComment(root->left, id, output);


}

int main(int argc, char* argv[]) {
    if (argc!= 4) {
        std::cout << "Not enough arguments" << std::endl;
        exit(1);
    }

    const std::string input1string = argv[1];
    const std::string input2string = argv[2];
    const std::string outputstring = argv[3];

    // open files
    std::ifstream input1(input1string);
    std::ifstream input2(input2string);
    std::ofstream output(outputstring);

    // check files are open
    if (!input1.is_open()) {
        std::cerr << "Failed to open the input1 file." << std::endl;
        exit(1);
    } if (!input2.is_open()) {
        std::cerr << "Failed to open the input2 file." << std::endl;
        exit(1);
    } if (!output.is_open()) {
        std::cerr << "Failed to open the output file." << std::endl;
        exit(1);
    }

    // create tree
    TreeNode* root;

    // parse input1
    std::string line, word;
    bool initial=true;
    while (std::getline(input1, line)) {
        std::string author, comID, parID, date, comment1, previous;
        int likes, replies;

        std::istringstream iss(line);
        bool first=true;
        bool iscomment=false;
        bool isdate=false;
        // finds different fields by looking at the previous word
        while (iss >> word) {
            if (first) {
                first=false;
            }
            else {
                if (iscomment) {
                    comment1 += " " +word;
                }
                else if (isdate) {
                    if (word=="\"crawled_date\":") {
                        isdate = false;
                    }
                    else {
                        date+=" " + word;
                    }
                }
                else {
                    if (previous=="\"like_count\":") {
                        likes = std::stoi(word);
                    }
                    else if (previous=="\"reply_count\":") {
                        replies = std::stoi(word);
                    }
                    else if (previous=="\"comment\":") {
                        comment1 += word;
                        iscomment=true;
                    }
                    else if (previous=="\"published_date\":") {
                        date = word;
                        isdate=true;
                    }
                    else if (previous=="\"parent_comment_id\":") {
                        if (word.size()==3) {
                            word="";
                        }
                    }
                    else {
                        if (previous=="\"author\":" or previous=="\"comment_id\":") {
                            // remove quotes and comma
                            word = word.substr(1, word.size()-3);
                            if (previous=="\"author\":") {
                                author=word;
                            }
                            if (previous=="\"comment_id\":") {
                                comID = word;
                            }
                        }
                    }
                }
            }
            previous=word;
        }
        // remove quotes and }
        comment1 = comment1.substr(1, comment1.size()-3);
        date = date.substr(1, date.size()-3);

        // create class and adds to tree
        Comment comm = Comment(author, comID, parID, date, comment1, likes, replies);
        if (initial) {
            initial=false;
            root = new TreeNode(comm);
        }
        else {
            bool found = false;
            addNode(root, comm, found);
        }
    }
    input1.close();

    // parse input2 file
    while (std::getline(input2, line)) {
        std::string command, parent_id, id2, author2, comment2, user;
        std::istringstream iss2(line);
        int index=-1;
        bool iscomment = false;
        while (iss2 >> word) {
            index++;
            if (index==0) {
                command = word;
            }
            // reply to video
            if (command == "reply_to_video") {
                if (iscomment) {
                    comment2 += " " +word;
                }
                else if (index==1) {
                    id2=word;
                }
                else if (index==2) {
                    author2 = word;
                }
                else {
                    iscomment=true;
                    comment2 = word;
                }
            }
            // reply to comment
            if (command == "reply_to_comment") {
                if (iscomment) {
                    comment2 += " " +word;
                }
                else if (index==1) {
                    parent_id=word;
                }
                else if (index==2) {
                    id2 = word;
                }
                else if (index==3) {
                    author2 = word;
                }
                else {
                    iscomment = true;
                    comment2 = word;
                }
            }
            if (iscomment) {
                comment2 = comment2.substr(1, comment2.size()-2);
            }
            // like comment
            if (command == "like_comment") {
                if (index==1) {
                    id2=word;
                }
            }
            // delete comment
            if (command == "delete_comment") {
                if (index==1) {
                    id2=word;
                }
            }
            // display comment
            if (command == "display_comment") {
                if (index==1) {
                    id2=word;
                }
            }
        }
        // create class depending on given info
        bool find = false;
        if (command == "reply_to_video") {
            Comment newcom = Comment(author2, id2, "", "0 seconds ago", comment2, 0, 0);
            addNode(root, newcom, find);
        }
        if (command == "reply_to_comment") {
            Comment newcom = Comment(author2, id2, parent_id, "0 seconds ago", comment2, 0, 0);
            addNode(root, newcom, find);
        }
        if (command == "like_comment") {
            likeComment(root, id2);
        }
        if (command == "delete_comment") {
            deleteComment(root, id2);
        }
        if (command == "display_comment") {
            displayComment(root, id2, output);
        }
    }

    // delete memory
    deleteTree(root);

    return 0;
}