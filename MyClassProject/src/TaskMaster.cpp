/* Program name: jbtictactoe.cpp
* Author: John Doe
* Date last updated: 5/1/2017
* Purpose: Play the game of Tic-Tac-Toe
*/ 
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>

// Enumerations for Category, Priority, and Status
enum class Category {
    Work = 1,
    Personal,
    Urgent
};

enum class Priority {
    Low = 1,
    Medium,
    High
};

enum class Status {
    Pending = 1,
    InProgress,
    Completed
};

// Function to convert string to Category enum
Category stringToCategory(const std::string& str) {
    if (str == "Work") return Category::Work;
    if (str == "Personal") return Category::Personal;
    return Category::Urgent;
}

// Function to convert Category enum to string
std::string categoryToString(Category category) {
    switch (category) {
        case Category::Work: return "Work";
        case Category::Personal: return "Personal";
        case Category::Urgent: return "Urgent";
        default: return "Unknown";
    }
}

// Similar conversion functions for Priority and Status
Priority stringToPriority(const std::string& str) {
    if (str == "Low") return Priority::Low;
    if (str == "Medium") return Priority::Medium;
    return Priority::High;
}

std::string priorityToString(Priority priority) {
    switch (priority) {
        case Priority::Low: return "Low";
        case Priority::Medium: return "Medium";
        case Priority::High: return "High";
        default: return "Unknown";
    }
}

Status stringToStatus(const std::string& str) {
    if (str == "Pending") return Status::Pending;
    if (str == "In Progress") return Status::InProgress;
    return Status::Completed;
}

std::string statusToString(Status status) {
    switch (status) {
        case Status::Pending: return "Pending";
        case Status::InProgress: return "In Progress";
        case Status::Completed: return "Completed";
        default: return "Unknown";
    }
}

// Task Class
class Task {
public:
    int id;
    std::string title;
    std::string description;
    Category category;
    Priority priority;
    std::string dueDate; // YYYY-MM-DD
    Status status;

    Task() : id(0), category(Category::Work), priority(Priority::Low), status(Status::Pending) {}

    // Serialize Task to a string for file storage
    std::string serialize() const {
        std::ostringstream oss;
        oss << id << "|" << title << "|" << description << "|" 
            << categoryToString(category) << "|" << priorityToString(priority) << "|" 
            << dueDate << "|" << statusToString(status);
        return oss.str();
    }

    // Deserialize Task from a string
    void deserialize(const std::string& data) {
        std::istringstream iss(data);
        std::string token;
        std::getline(iss, token, '|');
        id = std::stoi(token);
        std::getline(iss, title, '|');
        std::getline(iss, description, '|');
        std::getline(iss, token, '|');
        category = stringToCategory(token);
        std::getline(iss, token, '|');
        priority = stringToPriority(token);
        std::getline(iss, dueDate, '|');
        std::getline(iss, token, '|');
        status = stringToStatus(token);
    }

    // Display Task Details
    void display() const {
        std::cout << "ID: " << id << "\nTitle: " << title 
                  << "\nDescription: " << description 
                  << "\nCategory: " << categoryToString(category) 
                  << "\nPriority: " << priorityToString(priority) 
                  << "\nDue Date: " << dueDate 
                  << "\nStatus: " << statusToString(status) << "\n";
    }
};

// TaskManager Class
class TaskManager {
private:
    std::vector<Task> tasks;
    int nextId;

public:
    TaskManager() : nextId(1) {}

    // Create a new task
    void createTask() {
        Task task;
        task.id = nextId++;

        std::cin.ignore(); // Clear input buffer

        std::cout << "Enter Task Title: ";
        std::getline(std::cin, task.title);

        std::cout << "Enter Task Description: ";
        std::getline(std::cin, task.description);

        // Category Selection
        std::cout << "Choose Category:\n1. Work\n2. Personal\n3. Urgent\nEnter your choice: ";
        int catChoice;
        std::cin >> catChoice;
        task.category = static_cast<Category>(catChoice);

        // Priority Selection
        std::cout << "Choose Priority:\n1. Low\n2. Medium\n3. High\nEnter your choice: ";
        int priChoice;
        std::cin >> priChoice;
        task.priority = static_cast<Priority>(priChoice);

        // Due Date Input with basic validation
        std::cin.ignore(); // Clear input buffer
        while (true) {
            std::cout << "Enter Due Date (YYYY-MM-DD): ";
            std::getline(std::cin, task.dueDate);
            if (validateDate(task.dueDate)) break;
            else std::cout << "Invalid date format. Please try again.\n";
        }

        task.status = Status::Pending;
        tasks.push_back(task);
        std::cout << "Task \"" << task.title << "\" has been created successfully!\n";
    }

    // View All Tasks
    void viewTasks() const {
        if (tasks.empty()) {
            std::cout << "No tasks available.\n";
            return;
        }
        for (const auto& task : tasks) {
            task.display();
            std::cout << "-------------------------\n";
        }
    }

    // Edit a Task
    void editTask() {
        std::cout << "Enter Task ID to edit: ";
        int id;
        std::cin >> id;
        auto it = findTaskById(id);
        if (it == tasks.end()) {
            std::cout << "Task not found.\n";
            return;
        }

        std::cin.ignore(); // Clear input buffer

        std::cout << "Editing Task \"" << it->title << "\"\n";

        std::cout << "Enter new title (leave empty to keep current): ";
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty()) it->title = input;

        std::cout << "Enter new description (leave empty to keep current): ";
        std::getline(std::cin, input);
        if (!input.empty()) it->description = input;

        // Category Selection
        std::cout << "Choose Category (current: " << categoryToString(it->category) << "):\n1. Work\n2. Personal\n3. Urgent\nEnter your choice (0 to keep current): ";
        int catChoice;
        std::cin >> catChoice;
        if (catChoice >=1 && catChoice <=3)
            it->category = static_cast<Category>(catChoice);

        // Priority Selection
        std::cout << "Choose Priority (current: " << priorityToString(it->priority) << "):\n1. Low\n2. Medium\n3. High\nEnter your choice (0 to keep current): ";
        int priChoice;
        std::cin >> priChoice;
        if (priChoice >=1 && priChoice <=3)
            it->priority = static_cast<Priority>(priChoice);

        std::cin.ignore(); // Clear input buffer
        // Due Date Input with basic validation
        std::cout << "Enter new Due Date (YYYY-MM-DD) (leave empty to keep current): ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            if (validateDate(input))
                it->dueDate = input;
            else
                std::cout << "Invalid date format. Keeping current due date.\n";
        }

        // Status Update
        std::cout << "Choose Status (current: " << statusToString(it->status) << "):\n1. Pending\n2. In Progress\n3. Completed\nEnter your choice (0 to keep current): ";
        int statusChoice;
        std::cin >> statusChoice;
        if (statusChoice >=1 && statusChoice <=3)
            it->status = static_cast<Status>(statusChoice);

        std::cout << "Task updated successfully!\n";
    }

    // Delete a Task
    void deleteTask() {
        std::cout << "Enter Task ID to delete: ";
        int id;
        std::cin >> id;
        auto it = std::remove_if(tasks.begin(), tasks.end(), [&](const Task& t) { return t.id == id; });
        if (it != tasks.end()) {
            tasks.erase(it, tasks.end());
            std::cout << "Task deleted successfully.\n";
        } else {
            std::cout << "Task not found.\n";
        }
    }

    // Save Tasks to File
    void saveTasks() const {
        std::cout << "Enter filename to save tasks: ";
        std::string filename;
        std::cin >> filename;
        std::ofstream ofs(filename);
        if (!ofs) {
            std::cout << "Error opening file for writing.\n";
            return;
        }
        for (const auto& task : tasks) {
            ofs << task.serialize() << "\n";
        }
        ofs.close();
        std::cout << "Tasks saved successfully to " << filename << ".\n";
    }

    // Load Tasks from File
    void loadTasks() {
        std::cout << "Enter filename to load tasks: ";
        std::string filename;
        std::cin >> filename;
        std::ifstream ifs(filename);
        if (!ifs) {
            std::cout << "Error opening file for reading.\n";
            return;
        }
        tasks.clear();
        std::string line;
        int maxId = 0;
        while (std::getline(ifs, line)) {
            if (line.empty()) continue;
            Task task;
            task.deserialize(line);
            tasks.push_back(task);
            if (task.id > maxId) maxId = task.id;
        }
        nextId = maxId + 1;
        ifs.close();
        std::cout << "Tasks loaded successfully from " << filename << ".\n";
    }

    // Search and Filter Tasks
    void searchFilterTasks() const {
        std::cout << "Search and Filter Options:\n1. By Category\n2. By Priority\n3. By Status\nEnter your choice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear input buffer
        std::vector<Task> results;

        switch (choice) {
            case 1: {
                std::cout << "Choose Category:\n1. Work\n2. Personal\n3. Urgent\nEnter your choice: ";
                int catChoice;
                std::cin >> catChoice;
                Category category = static_cast<Category>(catChoice);
                for (const auto& task : tasks) {
                    if (task.category == category)
                        results.push_back(task);
                }
                break;
            }
            case 2: {
                std::cout << "Choose Priority:\n1. Low\n2. Medium\n3. High\nEnter your choice: ";
                int priChoice;
                std::cin >> priChoice;
                Priority priority = static_cast<Priority>(priChoice);
                for (const auto& task : tasks) {
                    if (task.priority == priority)
                        results.push_back(task);
                }
                break;
            }
            case 3: {
                std::cout << "Choose Status:\n1. Pending\n2. In Progress\n3. Completed\nEnter your choice: ";
                int statusChoice;
                std::cin >> statusChoice;
                Status status = static_cast<Status>(statusChoice);
                for (const auto& task : tasks) {
                    if (task.status == status)
                        results.push_back(task);
                }
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
                return;
        }

        if (results.empty()) {
            std::cout << "No tasks match the criteria.\n";
            return;
        }

        for (const auto& task : results) {
            task.display();
            std::cout << "-------------------------\n";
        }
    }

private:
    // Find Task by ID
    std::vector<Task>::iterator findTaskById(int id) {
        return std::find_if(tasks.begin(), tasks.end(), [&](const Task& t) { return t.id == id; });
    }

    // Basic Date Validation (YYYY-MM-DD)
    bool validateDate(const std::string& date) const {
        if (date.size() != 10) return false;
        if (date[4] != '-' || date[7] != '-') return false;
        // Additional validation can be added here (e.g., check if numbers are valid)
        return true;
    }
};

int main() {
    TaskManager manager;
    int choice;

    std::cout << "Welcome to TaskMaster!\n-----------------------\n";

    while (true) {
        std::cout << "\nPlease choose an option:\n"
                  << "1. Create New Task\n"
                  << "2. View All Tasks\n"
                  << "3. Edit Task\n"
                  << "4. Delete Task\n"
                  << "5. Save Tasks\n"
                  << "6. Load Tasks\n"
                  << "7. Search/Filter Tasks\n"
                  << "8. Exit\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                manager.createTask();
                break;
            case 2:
                manager.viewTasks();
                break;
            case 3:
                manager.editTask();
                break;
            case 4:
                manager.deleteTask();
                break;
            case 5:
                manager.saveTasks();
                break;
            case 6:
                manager.loadTasks();
                break;
            case 7:
                manager.searchFilterTasks();
                break;
            case 8:
                std::cout << "Exiting TaskMaster. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
