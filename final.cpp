#include <iostream>
#include <vector>
#include <random>

using namespace std;

int thresold;

// Function to evaluate a bivariate polynomial
vector<int> evaluatePolynomial(vector<vector<int>> coefficients, int x) {
    vector<int> sharecoef;
    for(int i=0;i<coefficients.size();i++){
        int valx = 1, res=0;
        for(int j=0;j<coefficients.size();j++){
            res += coefficients[j][i]*valx;
            valx *= x;
        }
        sharecoef.push_back(res);
    }
    return sharecoef;
}

// Function to generate random polynomial coefficients
vector<vector<int>> generateRandomCoefficients(int t) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 100);

    // Ensure constant term is the secret (can be modified for different secret placement)
    vector<vector<int>> coefficients(t,vector<int>(t,0));

    for (int i = 0; i < t; i++) {
        for (int j = 0; j <=i; j++) {
            int rn = dis(gen);
            coefficients[i][j] = coefficients[j][i] = rn;
        }
    }
    return coefficients;

}

// Function to generate shares for participants
vector<vector<int>> generateShares(vector<vector<int>> coefficients, vector<int> participants) {
    vector<vector<int>> shares;
    for (auto participant : participants) {
        int x = participant;
        shares.push_back(evaluatePolynomial(coefficients, x));
    }
    return shares;
}

// Function to reconstruct the secret using Lagrange interpolation (basic implementation)
long long reconstructSecret(vector<pair<int, vector<int>>> participants) {

    long long secret = 0;
    int n = participants.size();

    if(n==thresold){
        for(int i=0; i<participants.size(); i++) {
            long long numerator =1, denominator =1;
            for(int j=0;j<participants.size();j++){
                if(i!=j){
                    numerator = numerator * (-participants[j].first);
                    denominator = denominator * (participants[i].first - participants[j].first);
                }
            }
            secret += participants[i].second[0] * numerator / denominator ;
        }
    }
    return secret;
}

vector<int> genparticipants(int n){

    // random_device rd;
    // mt19937 gen(rd());
    // uniform_int_distribution<> dis(-10, 10);

    vector<int> participants;
    for(int i=0;i<n; i++){
        participants.push_back(i+1);
        // int rn1 = dis(gen)
        // participants.push_back({rn2});
    }
    return participants;

}

int main() {
    int n, t;

    // Get user input for n and t
    cout << "\n Enter the number of participants (n): ";
    cin >> n;
    cout << " Enter the threshold (t): ";
    cin >> t;

    thresold = t;
    // Ensure t <= n and t >= 2 (adjust as needed)
    if (t > n || t < 2 || n > (2*t - 1)) {
        cout << "Invalid threshold. Please enter a value between 2 and " << n << endl;
        return 1;
    }

    // Define participant coordinates
    vector<int> participants = genparticipants(n);

    // Generate random polynomial coefficients
    vector<vector<int>> coefficients = generateRandomCoefficients(t);

    cout<<" Generated Coefficients : \n";
    for(auto a:coefficients){
        for(auto b:a){
            cout<<" "<<b;
        }
        cout<<endl;
    }
    
    

    cout<<" \n set secret = ";
    int sec; cin>>sec;
    coefficients[0][0] = sec;

    // Generate shares
    vector<vector<int>> shares = generateShares(coefficients, participants);

    cout<<" Generated Shares : \n";
    int kk=1;
    for(auto v: shares){
        cout<<" Participant "<<kk++<<" : ";
        for(auto l:v){
            cout<<l<<" ";
        }
        cout<<endl;
    }

    // Simulate reconstruction with a subset of participants (replace with actual participants)
    char chr;
    do{
        cout<<"\n Enter the size of participants to be Involved : ";
        int a;cin>>a;
        cout<<" Enter participants to be Involved : ";
        vector<pair<int,vector<int>>> reconstruction_participants;
        for(int i=0;i<a;i++){
            int t;cin>>t;
            reconstruction_participants.push_back({t,shares[t-1]});
        }
        // (Prompt user to select participants for reconstruction)

        long long reconstructed_secret = reconstructSecret(reconstruction_participants);
        cout << "\n Reconstructed Secret: " << sec << endl;

        cout<<"\n\n\n To continue with diffrent participants press y : ";
        cin>>chr;
    }while(chr=='y');

    return 0;
}