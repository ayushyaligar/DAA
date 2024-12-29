#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <set>
#include <queue>
#include <climits>  // Include for INT_MAX
using namespace std;

struct Project {
    char name[100];
    int urgency;
    int impact;
    int cost;
    int priority_score;
    tm start_date;
    tm end_date;
    vector<string> dependencies;
};

void swap(Project& a, Project& b) {
    Project t = a;
    a = b;
    b = t;
}

int calculate_priority_score(int urgency, int impact, int cost) {
    return (urgency * 1.5) + (impact * 1.2) - (cost * 0.5);
}

void load_from_file(Project projects[], int& project_count, unordered_map<string, int>& project_index) {
    ifstream file("projects.txt");
    if (!file.is_open()) {
        cerr << "File Not Found\n";
        return;
    }
    project_count = 0;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string name, start_date_str, end_date_str, deps;
        iss >> name >> projects[project_count].urgency >> projects[project_count].impact >> projects[project_count].cost >> start_date_str >> end_date_str;
        getline(iss, deps);
        // Remove leading space from deps
        deps.erase(0, deps.find_first_not_of(' '));
        // Parse dependencies
        stringstream ss(deps);
        string dep;
        while (getline(ss, dep, ',')) {
            projects[project_count].dependencies.push_back(dep);
        }

        sscanf(start_date_str.c_str(), "%d-%d-%d", &projects[project_count].start_date.tm_year,
               &projects[project_count].start_date.tm_mon, &projects[project_count].start_date.tm_mday);
        projects[project_count].start_date.tm_year -= 1900;
        projects[project_count].start_date.tm_mon -= 1;

        sscanf(end_date_str.c_str(), "%d-%d-%d", &projects[project_count].end_date.tm_year,
               &projects[project_count].end_date.tm_mon, &projects[project_count].end_date.tm_mday);
        projects[project_count].end_date.tm_year -= 1900;
        projects[project_count].end_date.tm_mon -= 1;

        strcpy(projects[project_count].name, name.c_str());
        projects[project_count].priority_score = calculate_priority_score(projects[project_count].urgency,
                                                                          projects[project_count].impact,
                                                                          projects[project_count].cost);
        project_index[name] = project_count;
        project_count++;
    }
    file.close();
}

void print_projects(Project projects[], int project_count) {
    if (project_count == 0) {
        cout << "No projects to display.\n";
        return;
    }
    for (int i = 0; i < project_count; i++) {
        cout << "Project: " << projects[i].name 
             << ", Priority Score: " << projects[i].priority_score << endl;
    }
}

int partition(Project projects[], int l, int r) {
    int pivot = projects[l].priority_score;
    int i = l;
    int j = r + 1;

    while (true) {
        while (projects[++i].priority_score > pivot) {
            if (i == r) break;
        }
        while (projects[--j].priority_score < pivot) {
            if (j == l) break;
        }
        if (i >= j) break;
        swap(projects[i], projects[j]);
    }
    swap(projects[l], projects[j]);
    return j;
}

void quick_sort(Project projects[], int l, int r) {
    if (l < r) {
        int s = partition(projects, l, r);
        quick_sort(projects, l, s - 1);
        quick_sort(projects, s + 1, r);
    }
}

void add_project(Project projects[], int& project_count, unordered_map<string, int>& project_index) {
    cout << "Enter project name: ";
    cin.ignore();
    cin.getline(projects[project_count].name, 100);
    cout << "Enter urgency (1-10): ";
    cin >> projects[project_count].urgency;
    cout << "Enter impact (1-10): ";
    cin >> projects[project_count].impact;
    cout << "Enter cost (1-10): ";
    cin >> projects[project_count].cost;

    projects[project_count].priority_score = calculate_priority_score(projects[project_count].urgency,
                                                                      projects[project_count].impact,
                                                                      projects[project_count].cost);

    char start_date_str[11], end_date_str[11];
    cout << "Enter start date (YYYY-MM-DD): ";
    cin >> start_date_str;
    cout << "Enter end date (YYYY-MM-DD): ";
    cin >> end_date_str;

    sscanf(start_date_str, "%d-%d-%d", &projects[project_count].start_date.tm_year,
           &projects[project_count].start_date.tm_mon, &projects[project_count].start_date.tm_mday);
    projects[project_count].start_date.tm_year -= 1900;
    projects[project_count].start_date.tm_mon -= 1;

    sscanf(end_date_str, "%d-%d-%d", &projects[project_count].end_date.tm_year,
           &projects[project_count].end_date.tm_mon, &projects[project_count].end_date.tm_mday);
    projects[project_count].end_date.tm_year -= 1900;
    projects[project_count].end_date.tm_mon -= 1;

    string deps;
    cout << "Enter dependencies (comma separated): ";
    cin.ignore();
    getline(cin, deps);
    stringstream ss(deps);
    string dep;
    while (getline(ss, dep, ',')) {
        projects[project_count].dependencies.push_back(dep);
    }

    project_index[projects[project_count].name] = project_count;
    project_count++;
}

void filter_by_urgency(Project projects[], int project_count, int urgency) {
    cout << "Projects with urgency " << urgency << ":\n";
    for (int i = 0; i < project_count; i++) {
        if (projects[i].urgency == urgency) {
            cout << "Project: " << projects[i].name << ", Priority Score: " << projects[i].priority_score << endl;
        }
    }
}

void filter_by_cost(Project projects[], int project_count, int cost) {
    cout << "Projects with cost " << cost << ":\n";
    for (int i = 0; i < project_count; i++) {
        if (projects[i].cost == cost) {
            cout << "Project: " << projects[i].name << ", Priority Score: " << projects[i].priority_score << endl;
        }
    }
}

void filter_by_impact(Project projects[], int project_count, int impact) {
    cout << "Projects with impact " << impact << ":\n";
    for (int i = 0; i < project_count; i++) {
        if (projects[i].impact == impact) {
            cout << "Project: " << projects[i].name << ", Priority Score: " << projects[i].priority_score << endl;
        }
    }
}

void remove_project(Project projects[], int& project_count, const char* project_name, unordered_map<string, int>& project_index) {
    bool found = false;
    for (int i = 0; i < project_count; i++) {
        if (strcmp(projects[i].name, project_name) == 0) {
            found = true;
            for (int j = i; j < project_count - 1; j++) {
                projects[j] = projects[j + 1];
            }
            project_index.erase(project_name);
            project_count--;
            break;
        }
    }
    if (found) {
        cout << "Project " << project_name << " removed successfully.\n";
    } else {
        cout << "Project " << project_name << " not found.\n";
    }
}

void calculate_project_dates(Project projects[], int project_count) {
    time_t now = time(0);
    tm* current_date = localtime(&now);
    
    for (int i = 0; i < project_count; i++) {
        projects[i].start_date = *current_date;
        projects[i].end_date = *current_date;
        
        projects[i].end_date.tm_mday += 30;
        mktime(&projects[i].end_date);
        
        cout << "Project: " << projects[i].name << "\n";
        cout << "  Start Date: " << (1900 + projects[i].start_date.tm_year) << "-"
             << (projects[i].start_date.tm_mon + 1) << "-"
             << projects[i].start_date.tm_mday << "\n";
        cout << "  End Date: " << (1900 + projects[i].end_date.tm_year) << "-"
             << (projects[i].end_date.tm_mon + 1) << "-"
             << projects[i].end_date.tm_mday << "\n";
    }
}

void search_project(Project projects[], int project_count, const char* project_name) {
    string search_string = project_name;
    transform(search_string.begin(), search_string.end(), search_string.begin(), ::tolower);

    bool found = false;
    for (int i = 0; i < project_count; i++) {
        string project_name_str = projects[i].name;
        replace(project_name_str.begin(), project_name_str.end(), '_', ' ');
        transform(project_name_str.begin(), project_name_str.end(), project_name_str.begin(), ::tolower);

        if (project_name_str.find(search_string) != string::npos) {
            found = true;
            cout << "Project: " << projects[i].name << "\n";
            cout << "  Urgency: " << projects[i].urgency << "\n";
            cout << "  Impact: " << projects[i].impact << "\n";
            cout << "  Cost: " << projects[i].cost << "\n";
            cout << "  Priority Score: " << projects[i].priority_score << "\n";
            cout << "  Start Date: " << (1900 + projects[i].start_date.tm_year) << "-"
                 << (projects[i].start_date.tm_mon + 1) << "-"
                 << projects[i].start_date.tm_mday << "\n";
            cout << "  End Date: " << (1900 + projects[i].end_date.tm_year) << "-"
                 << (projects[i].end_date.tm_mon + 1) << "-"
                 << projects[i].end_date.tm_mday << "\n";
        }
    }
    if (!found) {
        cout << "No projects found with the keyword '" << project_name << "'.\n";
    }
}

// Function to calculate the optimal path using Dijkstra's algorithm
void dijkstra(Project projects[], int project_count, unordered_map<string, int>& project_index, const string& start_project) {
    if (project_index.find(start_project) == project_index.end()) {
        cout << "Error: The project '" << start_project << "' does not exist." << endl;
        return;
    }
    
    vector<int> dist(project_count, INT_MAX);  // Distance from start_project to each project
    vector<int> parent(project_count, -1);    // To store the path
    set<pair<int, int>> pq;  // (distance, project_index)

    int start_index = project_index[start_project];
    dist[start_index] = 0;
    pq.insert({0, start_index});

    while (!pq.empty()) {
        int u = pq.begin()->second;
        pq.erase(pq.begin());

        for (const string& dep : projects[u].dependencies) {
            if (project_index.find(dep) == project_index.end()) {
                cout << "Warning: Dependency '" << dep << "' for project '" << projects[u].name << "' does not exist." << endl;
                continue;
            }
            int v = project_index[dep];
            int weight = projects[v].priority_score * -1;  // Use negative priority score as weight

            if (dist[u] + weight < dist[v]) {
                pq.erase({dist[v], v});
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.insert({dist[v], v});
            }
        }
    }

    cout << "Optimal path from " << start_project << ":\n";
    for (int i = 0; i < project_count; i++) {
        if (dist[i] != INT_MAX) {
            cout << "Project: " << projects[i].name << ", Distance: " << -dist[i] << "\n";
        }
    }
}

void display_statistics(Project projects[], int project_count) {
    int high_priority = 0, medium_priority = 0, low_priority = 0;

    for (int i = 0; i < project_count; i++) {
        if (projects[i].priority_score > 15) high_priority++;
        else if (projects[i].priority_score >= 10) medium_priority++;
        else low_priority++;
    }

    cout << "\nProject Priority Distribution:\n";
    cout << "High Priority: " << high_priority << "\n";
    cout << "Medium Priority: " << medium_priority << "\n";
    cout << "Low Priority: " << low_priority << "\n";
}

int main() {
    Project projects[100];
    int project_count = 0;
    unordered_map<string, int> project_index;

    load_from_file(projects, project_count, project_index);

    int choice;
    do {
        cout << "\n--- Menu ---\n";
        cout << "1. View Projects\n";
        cout << "2. Add Project\n";
        cout << "3. Sort Projects by Priority\n";
        cout << "4. Filter Projects by Urgency\n";
        cout << "5. Filter Projects by Cost\n";
        cout << "6. Filter Projects by Impact\n";
        cout << "7. Remove Project\n";
        cout << "8. Calculate Project Start and End Dates\n";
        cout << "9. Search Project\n";
        cout << "10. View Project Statistics\n";
        cout << "11. Find Optimal Path\n";
        cout << "12. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                print_projects(projects, project_count);
                break;
            case 2:
                add_project(projects, project_count, project_index);
                break;
            case 3:
                quick_sort(projects, 0, project_count - 1);
                cout << "Projects sorted by priority.\n";
                break;
            case 4:
                {
                    int urgency;
                    cout << "Enter urgency to filter by (1-10): ";
                    cin >> urgency;
                    filter_by_urgency(projects, project_count, urgency);
                }
                break;
            case 5:
                {
                    int cost;
                    cout << "Enter cost to filter by (1-10): ";
                    cin >> cost;
                    filter_by_cost(projects, project_count, cost);
                }
                break;
            case 6:
                {
                    int impact;
                    cout << "Enter impact to filter by (1-10): ";
                    cin >> impact;
                    filter_by_impact(projects, project_count, impact);
                }
                break;
            case 7:
                {
                    char project_name[100];
                    cout << "Enter project name to remove: ";
                    cin.ignore();
                    cin.getline(project_name, 100);
                    remove_project(projects, project_count, project_name, project_index);
                }
                break;
            case 8:
                calculate_project_dates(projects, project_count);
                break;
            case 9:
                {
                    char project_name[100];
                    cout << "Enter project name to search: ";
                    cin.ignore();
                    cin.getline(project_name, 100);
                    search_project(projects, project_count, project_name);
                }
                break;
            case 10:
                display_statistics(projects, project_count);
                break;
            case 11:
                {
                    string start_project;
                    cout << "Enter the name of the starting project: ";
                    cin.ignore();
                    getline(cin, start_project);
                    dijkstra(projects, project_count, project_index, start_project);
                }
                break;
            case 12:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 12);

    return 0;
}