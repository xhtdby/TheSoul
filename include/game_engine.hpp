#pragma once
#include <string>
#include <vector>
#include <map>
#include "instance.hpp"

// Represents a goal/constraint for the game
struct GameGoal {
    std::string type;  // "min_score", "min_money", "max_score", "max_money"
    double targetValue;
    int byAnteRound;  // The ante/round by which this goal should be achieved
    bool isMandatory;  // If true, game stops if goal cannot be met
    
    GameGoal() {
        type = "";
        targetValue = 0;
        byAnteRound = 1;
        isMandatory = false;
    }
    
    GameGoal(std::string t, double val, int ante, bool mandatory) {
        type = t;
        targetValue = val;
        byAnteRound = ante;
        isMandatory = mandatory;
    }
};

// Represents the current state of a game
struct GameState {
    int currentAnte;
    int currentRound;  // Round within ante (1-3 for small/big/boss)
    double score;
    double money;
    int handSize;
    int discards;
    int hands;
    std::vector<std::string> jokers;
    std::vector<std::string> consumables;
    std::vector<std::string> activeVouchers;
    std::string deck;
    std::string stake;
    
    GameState() {
        currentAnte = 1;
        currentRound = 1;
        score = 0;
        money = 0;
        handSize = 8;
        discards = 3;
        hands = 4;
        deck = "Red Deck";
        stake = "White Stake";
    }
};

// Game engine for simulation and goal checking
struct GameEngine {
    Instance* instance;
    GameState state;
    std::vector<GameGoal> goals;
    std::map<std::string, std::string> feedback;
    bool initialized;
    
    GameEngine() {
        instance = nullptr;
        initialized = false;
    }
    
    GameEngine(std::string seed) {
        instance = new Instance(seed);
        initialized = true;
    }
    
    ~GameEngine() {
        if (instance != nullptr) {
            delete instance;
        }
    }
    
    // Initialize with seed
    void initialize(std::string seed) {
        if (instance != nullptr) {
            delete instance;
        }
        instance = new Instance(seed);
        initialized = true;
    }
    
    // Set the current game state
    void setState(GameState newState) {
        state = newState;
        if (instance != nullptr) {
            instance->setDeck(newState.deck);
            instance->setStake(newState.stake);
            for (const auto& voucher : newState.activeVouchers) {
                instance->activateVoucher(voucher);
            }
        }
    }
    
    // Add a goal/constraint
    void addGoal(GameGoal goal) {
        goals.push_back(goal);
    }
    
    // Add a goal with simple parameters
    void addGoal(std::string type, double value, int byAnte, bool mandatory) {
        goals.push_back(GameGoal(type, value, byAnte, mandatory));
    }
    
    // Clear all goals
    void clearGoals() {
        goals.clear();
        feedback.clear();
    }
    
    // Get the next predicted items for the current ante
    std::vector<std::string> predictNextShopItems(int count, int ante) {
        std::vector<std::string> items;
        if (!initialized || instance == nullptr) {
            return items;
        }
        
        for (int i = 0; i < count; i++) {
            ShopItem item = instance->nextShopItem(ante);
            items.push_back(item.item);
        }
        return items;
    }
    
    // Get boss for an ante
    std::string predictBoss(int ante) {
        if (!initialized || instance == nullptr) {
            return "";
        }
        return instance->nextBoss(ante);
    }
    
    // Get voucher for an ante
    std::string predictVoucher(int ante) {
        if (!initialized || instance == nullptr) {
            return "";
        }
        return instance->nextVoucher(ante);
    }
    
    // Check if goals are feasible
    bool checkGoalFeasibility() {
        if (!initialized) {
            feedback["error"] = "Engine not initialized with seed";
            return false;
        }
        
        bool allFeasible = true;
        feedback.clear();
        
        for (const auto& goal : goals) {
            std::string key = goal.type + "_ante_" + std::to_string(goal.byAnteRound);
            
            if (goal.type == "min_score") {
                // Basic feasibility check - very simplified
                // In reality, score calculation is complex and depends on many factors
                if (goal.targetValue > 1000000 * goal.byAnteRound) {
                    feedback[key] = "UNLIKELY - Target score very high for ante " + std::to_string(goal.byAnteRound);
                    if (goal.isMandatory) allFeasible = false;
                } else {
                    feedback[key] = "POSSIBLE - Target score achievable with good strategy";
                }
            } else if (goal.type == "min_money") {
                // Money accumulation is more predictable
                // Base interest + shop sales + rewards
                double estimatedMoney = goal.byAnteRound * 15; // Very rough estimate
                if (goal.targetValue > estimatedMoney * 2) {
                    feedback[key] = "CHALLENGING - May need to sell items or get lucky";
                    if (goal.isMandatory && goal.targetValue > estimatedMoney * 4) {
                        allFeasible = false;
                    }
                } else {
                    feedback[key] = "FEASIBLE - Target money achievable";
                }
            } else if (goal.type == "max_money") {
                feedback[key] = "NOTED - Will track spending to stay under $" + std::to_string((int)goal.targetValue);
            } else if (goal.type == "max_score") {
                feedback[key] = "NOTED - Will avoid excessive scoring";
            }
        }
        
        return allFeasible;
    }
    
    // Get feedback messages
    std::string getFeedback() {
        std::string result = "";
        for (const auto& pair : feedback) {
            result += pair.first + ": " + pair.second + "\n";
        }
        return result;
    }
    
    // Simulate progression to a target ante
    std::string simulateToAnte(int targetAnte) {
        if (!initialized || instance == nullptr) {
            return "Error: Engine not initialized";
        }
        
        std::string report = "=== SIMULATION REPORT ===\n";
        report += "Seed: " + instance->seed + "\n";
        report += "Deck: " + state.deck + "\n";
        report += "Stake: " + state.stake + "\n\n";
        
        for (int ante = state.currentAnte; ante <= targetAnte; ante++) {
            report += "ANTE " + std::to_string(ante) + ":\n";
            
            // Boss
            std::string boss = instance->nextBoss(ante);
            report += "  Boss: " + boss + "\n";
            
            // Voucher
            std::string voucher = instance->nextVoucher(ante);
            report += "  Voucher: " + voucher + "\n";
            
            // Tags
            std::string tag1 = instance->nextTag(ante);
            std::string tag2 = instance->nextTag(ante);
            report += "  Tags: " + tag1 + ", " + tag2 + "\n";
            
            // Check goals for this ante
            for (const auto& goal : goals) {
                if (goal.byAnteRound == ante) {
                    std::string key = goal.type + "_ante_" + std::to_string(ante);
                    if (feedback.find(key) != feedback.end()) {
                        report += "  GOAL: " + feedback[key] + "\n";
                    }
                }
            }
            
            report += "\n";
        }
        
        return report;
    }
    
    // Get recommended actions based on goals
    std::vector<std::string> getRecommendedActions(int ante) {
        std::vector<std::string> recommendations;
        
        if (!initialized) {
            recommendations.push_back("Initialize engine with seed first");
            return recommendations;
        }
        
        // Analyze goals for this ante
        for (const auto& goal : goals) {
            if (goal.byAnteRound >= ante && goal.byAnteRound <= ante + 2) {
                if (goal.type == "min_money") {
                    recommendations.push_back("Save money for ante " + std::to_string(goal.byAnteRound) + 
                                            " goal: $" + std::to_string((int)goal.targetValue));
                } else if (goal.type == "min_score") {
                    recommendations.push_back("Focus on scoring jokers for ante " + std::to_string(goal.byAnteRound) + 
                                            " goal: " + std::to_string((int)goal.targetValue) + " chips");
                }
            }
        }
        
        // Check what's available in shop
        std::string boss = instance->nextBoss(ante);
        recommendations.push_back("Prepare for boss: " + boss);
        
        return recommendations;
    }
    
    // Query for specific information
    std::string query(std::string questionType, int ante) {
        if (!initialized || instance == nullptr) {
            return "Error: Engine not initialized";
        }
        
        if (questionType == "boss") {
            return instance->nextBoss(ante);
        } else if (questionType == "voucher") {
            return instance->nextVoucher(ante);
        } else if (questionType == "tag") {
            return instance->nextTag(ante);
        } else if (questionType == "pack") {
            return instance->nextPack(ante);
        }
        
        return "Unknown query type";
    }
};
