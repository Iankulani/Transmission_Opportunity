#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for sleep function

// Define the maximum number of nodes in the network
#define MAX_NODES 100

// Structure to represent each node in the network
typedef struct Node {
    char name[50];
    int txop_duration;  // Transmission Opportunity (TxOp) duration in seconds
    int busy;           // Flag to indicate if the node is currently busy
} Node;

// Structure to represent the network
typedef struct AdHocNetwork {
    Node nodes[MAX_NODES];
    int node_count;
} AdHocNetwork;

// Function to add a node to the network
void add_node(AdHocNetwork *network, const char *node_name) {
    if (network->node_count < MAX_NODES) {
        strcpy(network->nodes[network->node_count].name, node_name);
        network->nodes[network->node_count].txop_duration = 0;
        network->nodes[network->node_count].busy = 0;
        network->node_count++;
    } else {
        printf("Network is full! Cannot add more nodes.\n");
    }
}

// Function to set the TxOp duration for a specific node
void set_txop(AdHocNetwork *network, const char *node_name, int txop_duration) {
    for (int i = 0; i < network->node_count; i++) {
        if (strcmp(network->nodes[i].name, node_name) == 0) {
            network->nodes[i].txop_duration = txop_duration;
            return;
        }
    }
    printf("Node %s does not exist in the network.\n", node_name);
}

// Function to simulate data transmission from sender to receiver
void transmit(AdHocNetwork *network, const char *sender, const char *receiver) {
    int sender_index = -1, receiver_index = -1;
    
    // Find the sender and receiver in the network
    for (int i = 0; i < network->node_count; i++) {
        if (strcmp(network->nodes[i].name, sender) == 0) {
            sender_index = i;
        }
        if (strcmp(network->nodes[i].name, receiver) == 0) {
            receiver_index = i;
        }
    }

    if (sender_index == -1 || receiver_index == -1) {
        printf("Invalid sender or receiver.\n");
        return;
    }

    if (network->nodes[sender_index].busy) {
        printf("%s is currently busy and cannot transmit.\n", sender);
        return;
    }

    // Mark the sender as busy during the TxOp
    network->nodes[sender_index].busy = 1;
    int txop_duration = network->nodes[sender_index].txop_duration;

    // Simulate the transmission
    printf("%s is transmitting to %s...\n", sender, receiver);
    printf("Transmission will take %d seconds.\n", txop_duration);

    // Simulate the transmission by sleeping for the TxOp duration
    sleep(txop_duration);

    // Mark the sender as not busy after transmission
    network->nodes[sender_index].busy = 0;
    printf("%s successfully transmitted to %s.\n", sender, receiver);
}

// Function to display the current status of the network
void display_network(const AdHocNetwork *network) {
    printf("\nNetwork Status:\n");
    for (int i = 0; i < network->node_count; i++) {
        printf("Node: %s, TxOp: %d seconds, Busy: %s\n",
               network->nodes[i].name,
               network->nodes[i].txop_duration,
               network->nodes[i].busy ? "Yes" : "No");
    }
}

// Function to get user input for the network configuration
void get_user_input(AdHocNetwork *network) {
    int num_nodes;
    char node_name[50];

    // Get number of nodes
    printf("Enter the number of nodes in the network:");
    scanf("%d", &num_nodes);
    getchar();  // Clear newline character after input

    for (int i = 0; i < num_nodes; i++) {
        printf("Enter node name (computer/router):");
        fgets(node_name, sizeof(node_name), stdin);
        node_name[strcspn(node_name, "\n")] = 0;  // Remove trailing newline
        add_node(network, node_name);
    }

    // Set TxOp for each node
    for (int i = 0; i < network->node_count; i++) {
        int txop_duration;
        printf("Enter the TxOp duration (in seconds) for %s: ", network->nodes[i].name);
        scanf("%d", &txop_duration);
        set_txop(network, network->nodes[i].name, txop_duration);
    }

    // Get sender and receiver for transmission
    char sender[50], receiver[50];
    printf("\nEnter the sender node:");
    getchar();  // Clear newline character
    fgets(sender, sizeof(sender), stdin);
    sender[strcspn(sender, "\n")] = 0;  // Remove trailing newline

    printf("Enter the receiver node:");
    fgets(receiver, sizeof(receiver), stdin);
    receiver[strcspn(receiver, "\n")] = 0;  // Remove trailing newline

    // Simulate transmission
    transmit(network, sender, receiver);
}

int main() {
    AdHocNetwork network = { .node_count = 0 };
    
    printf("================= Ad-Hoc Network TxOp Simulation Tool =================\n\n");

    // Get user input and simulate network operations
    get_user_input(&network);

    // Display updated network status
    display_network(&network);

    return 0;
}
