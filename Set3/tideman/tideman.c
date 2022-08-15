#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX] = {{0}};

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool check_loop(int start, int end);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidates ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Parse candidates table to identify candidate ID to save it in ranks table
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(name, candidates[i]))
        {
            ranks[rank] = i;
            return true;
        }
    }

    // TODO : Exclude possibility to vote 2 times for the same candidate?

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Define and save preference of ballot of voter
    int better[candidate_count];
    memset(better, 99, candidate_count * sizeof(int));

    bool cancel = false;

    // Parse ranks table to record all preferences for each candidate by order of preference
    for (int i = 0; i < candidate_count; i++)
    {
        better[i] = ranks[i];

        // Update all candidate (j) who hasn't preferences over i
        for (int j = 0; j < candidate_count; j++)
        {
            for (int k = 0; k < candidate_count; k++)
            {
                // Except if j candidates was already treated
                if (better[k] == j)
                {
                    cancel = true;
                    break;
                }
            }
            if (cancel == false)
            {
                preferences[ranks[i]][j]++;
            }
            else
            {
                cancel = false;
            }
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Parse preferences of all voters to identify pairs. Skip diagonal of table i = j
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
            else
            {
                // Do nothting as preference is equal
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int max = 0, id_max, win_temp, los_temp;

        // Parse pairs to identify maximum
        for (int j = i; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] > max)
            {
                max = preferences[pairs[j].winner][pairs[j].loser];
                id_max = j;
            }
        }

        // Invert max with current position
        if (i != id_max)
        {
            win_temp = pairs[i].winner;
            los_temp = pairs[i].loser;
            pairs[i].winner = pairs[id_max].winner;
            pairs[i].loser = pairs[id_max].loser;
            pairs[id_max].winner = win_temp;
            pairs[id_max].loser = los_temp;
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Parse pairs array and record all arrow from winner to loser if this does not create a loop
    for (int i = 0; i < pair_count; i++)
    {
        if (!check_loop(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Check if a loop exist between start and end
bool check_loop(int start, int end)
{

    // If following item go back to start, declare a loop
    if (locked[end][start] == true)
    {
        return true;
    }

    // If following item go somewhere else, check this new point does not loop to the start
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[end][i] == true)
        {
            if (check_loop(start, i))
            {
                return true;
            }
        }
    }

    return false;
}


// Print the winner of the election
void print_winner(void)
{
    int count[candidate_count], winner = 0;

    // Count all destination arrow for each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == true)
            {
                count[j]++;
            }
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (count[winner] > count[i])
        {
            winner = i;
        }
    }

    printf("%s\n", candidates[winner]);

    return;
}