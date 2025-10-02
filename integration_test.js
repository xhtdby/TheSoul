/**
 * Integration Test for Game Engine
 * 
 * This script demonstrates and tests the Game Engine functionality.
 * Run this in a browser console after loading immolate.js
 */

console.log("=== Game Engine Integration Test ===\n");

// Test 1: Basic Initialization
console.log("Test 1: Basic Initialization");
try {
    const engine = new Module.GameEngine("TEST123");
    const state = new Module.GameState();
    state.deck = "Red Deck";
    state.stake = "White Stake";
    engine.setState(state);
    console.log("✓ Engine initialized successfully");
    console.log("  Seed:", engine.state.deck);
    console.log("");
} catch (e) {
    console.error("✗ Initialization failed:", e.message);
}

// Test 2: Adding Goals
console.log("Test 2: Adding Goals");
try {
    const engine = new Module.GameEngine("GOAL123");
    const state = new Module.GameState();
    engine.setState(state);
    
    // Add a money goal
    const goal1 = new Module.GameGoal("min_money", 100, 3, true);
    engine.addGoal(goal1);
    console.log("✓ Added min_money goal: $100 by ante 3");
    
    // Add a score goal
    const goal2 = new Module.GameGoal("min_score", 500000, 5, false);
    engine.addGoal(goal2);
    console.log("✓ Added min_score goal: 500k by ante 5");
    console.log("");
} catch (e) {
    console.error("✗ Adding goals failed:", e.message);
}

// Test 3: Feasibility Checking
console.log("Test 3: Feasibility Checking");
try {
    const engine = new Module.GameEngine("FEASBL1");
    const state = new Module.GameState();
    engine.setState(state);
    
    engine.addGoal(new Module.GameGoal("min_money", 100, 3, true));
    engine.addGoal(new Module.GameGoal("min_score", 500000, 5, false));
    
    const feasible = engine.checkGoalFeasibility();
    console.log("✓ Feasibility check completed");
    console.log("  All mandatory goals feasible:", feasible);
    
    const feedback = engine.getFeedback();
    console.log("  Feedback:\n" + feedback);
} catch (e) {
    console.error("✗ Feasibility check failed:", e.message);
}

// Test 4: Simulation
console.log("Test 4: Running Simulation");
try {
    const engine = new Module.GameEngine("SIMUL01");
    const state = new Module.GameState();
    state.deck = "Blue Deck";
    state.stake = "Red Stake";
    engine.setState(state);
    
    engine.addGoal(new Module.GameGoal("min_money", 150, 4, true));
    
    const report = engine.simulateToAnte(3);
    console.log("✓ Simulation completed");
    console.log("  Report preview:");
    console.log(report.substring(0, 300) + "...");
    console.log("");
} catch (e) {
    console.error("✗ Simulation failed:", e.message);
}

// Test 5: Querying Information
console.log("Test 5: Querying Information");
try {
    const engine = new Module.GameEngine("QUERY01");
    engine.initialize("QUERY01");
    
    const boss = engine.query("boss", 1);
    console.log("✓ Query executed");
    console.log("  Boss for ante 1:", boss);
    
    const voucher = engine.query("voucher", 2);
    console.log("  Voucher for ante 2:", voucher);
    console.log("");
} catch (e) {
    console.error("✗ Query failed:", e.message);
}

// Test 6: Getting Recommendations
console.log("Test 6: Getting Recommendations");
try {
    const engine = new Module.GameEngine("RECOMM1");
    const state = new Module.GameState();
    engine.setState(state);
    
    engine.addGoal(new Module.GameGoal("min_money", 200, 5, true));
    engine.addGoal(new Module.GameGoal("min_score", 800000, 6, false));
    
    const recommendations = engine.getRecommendedActions(2);
    console.log("✓ Recommendations generated");
    console.log("  Number of recommendations:", recommendations.size());
    for (let i = 0; i < recommendations.size(); i++) {
        console.log("  -", recommendations.get(i));
    }
    console.log("");
} catch (e) {
    console.error("✗ Getting recommendations failed:", e.message);
}

// Test 7: Mid-Game State
console.log("Test 7: Mid-Game State Management");
try {
    const engine = new Module.GameEngine("MIDGAM1");
    const state = new Module.GameState();
    state.currentAnte = 3;
    state.money = 85;
    state.score = 250000;
    state.handSize = 8;
    state.discards = 4;
    state.hands = 4;
    engine.setState(state);
    
    console.log("✓ Mid-game state set");
    console.log("  Current ante:", state.currentAnte);
    console.log("  Money:", state.money);
    console.log("  Score:", state.score);
    
    engine.addGoal(new Module.GameGoal("min_money", 150, 5, true));
    const feasible = engine.checkGoalFeasibility();
    console.log("  Can reach $150 by ante 5:", feasible ? "YES" : "MAYBE");
    console.log("");
} catch (e) {
    console.error("✗ Mid-game state test failed:", e.message);
}

// Test 8: Multiple Seeds Comparison
console.log("Test 8: Multiple Seeds Comparison");
try {
    const seeds = ["WINNER1", "LOSER01", "RANDOM1"];
    const goal = new Module.GameGoal("min_money", 150, 4, true);
    
    console.log("✓ Testing multiple seeds");
    for (const seed of seeds) {
        const engine = new Module.GameEngine(seed);
        const state = new Module.GameState();
        engine.setState(state);
        engine.addGoal(goal);
        const feasible = engine.checkGoalFeasibility();
        console.log("  Seed:", seed, "- Feasible:", feasible ? "✓" : "?");
    }
    console.log("");
} catch (e) {
    console.error("✗ Multiple seeds test failed:", e.message);
}

// Test 9: Prediction Methods
console.log("Test 9: Testing Prediction Methods");
try {
    const engine = new Module.GameEngine("PREDICT");
    engine.initialize("PREDICT");
    
    const boss = engine.predictBoss(1);
    console.log("✓ Predictions working");
    console.log("  Boss for ante 1:", boss);
    
    const voucher = engine.predictVoucher(1);
    console.log("  Voucher for ante 1:", voucher);
    
    const items = engine.predictNextShopItems(5, 1);
    console.log("  First 5 shop items:", items.size());
    console.log("");
} catch (e) {
    console.error("✗ Prediction methods failed:", e.message);
}

// Test 10: Stress Test
console.log("Test 10: Stress Test (Multiple Operations)");
try {
    const engine = new Module.GameEngine("STRESS1");
    const state = new Module.GameState();
    engine.setState(state);
    
    // Add many goals
    for (let i = 1; i <= 8; i++) {
        engine.addGoal(new Module.GameGoal("min_money", 50 * i, i, false));
    }
    
    console.log("✓ Added 8 goals");
    
    // Check feasibility
    engine.checkGoalFeasibility();
    console.log("✓ Feasibility checked");
    
    // Simulate
    const report = engine.simulateToAnte(8);
    console.log("✓ Simulated 8 antes");
    
    // Clear and re-add
    engine.clearGoals();
    console.log("✓ Cleared goals");
    
    engine.addGoal(new Module.GameGoal("min_score", 1000000, 5, true));
    console.log("✓ Added new goal after clear");
    console.log("");
} catch (e) {
    console.error("✗ Stress test failed:", e.message);
}

console.log("=== All Tests Completed ===");
console.log("\nNote: Some tests may show warnings or '?' results depending on");
console.log("the specific seed outcomes. This is expected behavior.");
console.log("\nTo use the Game Engine in your code:");
console.log("  1. Create engine: new Module.GameEngine(seed)");
console.log("  2. Set state: engine.setState(state)");
console.log("  3. Add goals: engine.addGoal(goal)");
console.log("  4. Check: engine.checkGoalFeasibility()");
console.log("  5. Simulate: engine.simulateToAnte(ante)");
