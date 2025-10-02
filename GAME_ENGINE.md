# Balatro Game Engine

## Overview

The Game Engine is a flexible system that extends the seed analyzer to help players plan their Balatro runs by:

1. Setting goals and constraints (e.g., "have at least $100 by ante 3")
2. Checking if goals are feasible with the given seed
3. Simulating game progression to any ante
4. Providing recommendations based on your goals
5. Allowing you to feed in game state at any point and continue analysis

## Features

### 1. Goal Management
Set various types of goals for your run:
- **min_money**: Minimum money target by a certain ante
- **min_score**: Minimum score target by a certain ante  
- **max_money**: Maximum money constraint (e.g., for challenges)
- **max_score**: Maximum score constraint

Each goal can be marked as mandatory or optional. Mandatory goals will flag the run as potentially unfeasible if they seem unachievable.

### 2. State Management
Feed in your current game state at any point:
- Current ante and round
- Money and score
- Active jokers and consumables
- Vouchers collected
- Hand size, discards, and hands remaining

The engine will then analyze the remainder of the run from that point forward.

### 3. Simulation
Simulate your run progression to any ante, getting:
- Boss blinds for each ante
- Vouchers available
- Tags received
- Pack types
- Shop items (via existing seed analysis)
- Goal achievement status

### 4. Feasibility Checking
The engine analyzes your goals against the seed and provides feedback:
- Whether goals are achievable
- If goals are challenging or unlikely
- Recommendations on strategy
- Warnings if mandatory goals cannot be met

### 5. Query Interface
Ask specific questions about the seed:
- What boss will I face in ante X?
- What voucher will appear in ante X?
- What tags will I get?
- What pack types will appear?

## Usage Examples

### JavaScript/Web Interface

```javascript
// Initialize the engine with a seed
let engine = new Module.GameEngine("ABCD1234");

// Set up initial game state
let state = new Module.GameState();
state.deck = "Red Deck";
state.stake = "White Stake";
state.currentAnte = 1;
engine.setState(state);

// Add goals
engine.addGoal(new Module.GameGoal("min_money", 100, 3, true));  // $100 by ante 3 (mandatory)
engine.addGoal(new Module.GameGoal("min_score", 50000, 5, false)); // 50k score by ante 5 (optional)

// Check if goals are feasible
let feasible = engine.checkGoalFeasibility();
console.log(engine.getFeedback());

// Simulate to ante 5
let report = engine.simulateToAnte(5);
console.log(report);

// Query specific information
let boss = engine.query("boss", 3);  // What boss in ante 3?
let voucher = engine.query("voucher", 2);  // What voucher in ante 2?

// Get recommendations
let recommendations = engine.getRecommendedActions(2);
for (let i = 0; i < recommendations.size(); i++) {
    console.log(recommendations.get(i));
}
```

### C++ API

```cpp
#include "game_engine.hpp"

// Create engine
GameEngine engine("SEEDTEXT");

// Set state
GameState state;
state.deck = "Red Deck";
state.stake = "White Stake";
state.currentAnte = 1;
state.money = 50;
engine.setState(state);

// Add goals
engine.addGoal("min_money", 150, 4, true);

// Check feasibility
bool feasible = engine.checkGoalFeasibility();
std::string feedback = engine.getFeedback();

// Simulate
std::string report = engine.simulateToAnte(5);
```

## API Reference

### GameEngine Class

#### Constructor
```cpp
GameEngine()  // Empty constructor
GameEngine(std::string seed)  // Initialize with seed
```

#### Methods

**initialize(seed)**
- Initialize or re-initialize the engine with a new seed
- Parameters: `seed` (string) - 8 character seed code

**setState(state)**
- Set the current game state
- Parameters: `state` (GameState) - Complete game state

**addGoal(goal)** or **addGoal(type, value, ante, mandatory)**
- Add a goal or constraint to track
- Parameters:
  - `type` (string): "min_money", "min_score", "max_money", or "max_score"
  - `value` (double): Target value
  - `ante` (int): Ante by which goal should be achieved
  - `mandatory` (bool): Whether this goal is required

**clearGoals()**
- Remove all goals

**checkGoalFeasibility()**
- Analyze if goals are achievable
- Returns: bool - true if all mandatory goals seem feasible

**getFeedback()**
- Get detailed feedback about goals
- Returns: string - Multi-line feedback report

**simulateToAnte(targetAnte)**
- Simulate game progression to a target ante
- Parameters: `targetAnte` (int) - Ante to simulate to
- Returns: string - Detailed simulation report

**getRecommendedActions(ante)**
- Get strategic recommendations for current ante
- Parameters: `ante` (int) - Current ante
- Returns: vector<string> - List of recommendations

**query(questionType, ante)**
- Query specific information about the seed
- Parameters:
  - `questionType` (string): "boss", "voucher", "tag", or "pack"
  - `ante` (int): Ante to query about
- Returns: string - Query result

**predictBoss(ante)**
**predictVoucher(ante)**
**predictNextShopItems(count, ante)**
- Specific prediction methods
- See method signatures for details

### GameState Structure

```cpp
struct GameState {
    int currentAnte;       // Current ante (1-8)
    int currentRound;      // Round within ante (1-3)
    double score;          // Current score
    double money;          // Current money
    int handSize;          // Cards in hand
    int discards;          // Discards available
    int hands;             // Hands available
    vector<string> jokers;           // Active jokers
    vector<string> consumables;      // Consumables in inventory
    vector<string> activeVouchers;   // Purchased vouchers
    string deck;           // Deck type
    string stake;          // Stake level
};
```

### GameGoal Structure

```cpp
struct GameGoal {
    string type;           // Goal type
    double targetValue;    // Target value to achieve
    int byAnteRound;      // Ante by which to achieve
    bool isMandatory;     // Whether goal is required
};
```

## Example Scenarios

### Scenario 1: Money Rush Challenge
"I need to have at least $200 by ante 4 and not spend more than $50 before ante 3."

```javascript
let engine = new Module.GameEngine("MNYRUN01");
let state = new Module.GameState();
state.deck = "Yellow Deck";  // Starts with extra money
engine.setState(state);

engine.addGoal(new Module.GameGoal("min_money", 200, 4, true));
engine.addGoal(new Module.GameGoal("max_money", 50, 3, false));  // Don't go below $50

if (engine.checkGoalFeasibility()) {
    console.log("✓ This seed can achieve the money rush!");
    console.log(engine.simulateToAnte(4));
} else {
    console.log("✗ This seed may struggle with money goals");
}
```

### Scenario 2: Mid-Run Analysis
"I'm at ante 3 with 3 jokers and $80. Can I reach 1 million score by ante 5?"

```javascript
let engine = new Module.GameEngine("CURRENT1");
let state = new Module.GameState();
state.currentAnte = 3;
state.money = 80;
state.score = 250000;
state.jokers = ["Blueprint", "Brainstorm", "Baron"];
engine.setState(state);

engine.addGoal(new Module.GameGoal("min_score", 1000000, 5, true));
engine.checkGoalFeasibility();
console.log(engine.getFeedback());
```

### Scenario 3: Seed Shopping
"I want a seed where I can get a DNA joker early and have at least $150 by ante 3."

```javascript
// Test multiple seeds
for (let seed of ["SEED0001", "SEED0002", "SEED0003"]) {
    let engine = new Module.GameEngine(seed);
    let state = new Module.GameState();
    engine.setState(state);
    
    engine.addGoal(new Module.GameGoal("min_money", 150, 3, true));
    
    // Check if DNA appears early
    let items = engine.predictNextShopItems(50, 1);
    let hasDNA = items.includes("DNA");
    
    let feasible = engine.checkGoalFeasibility();
    
    if (hasDNA && feasible) {
        console.log("✓ Found good seed: " + seed);
        break;
    }
}
```

## Integration with Existing UI

The game engine can be integrated into the existing TheSoul interface by adding a new section for goal-based analysis. The `game_engine_example.html` file provides a standalone interface that demonstrates all features.

To integrate into `index.html`:
1. Add goal input fields
2. Add buttons for feasibility checking and simulation
3. Display results alongside existing analysis
4. Allow users to set constraints before running analysis

## Future Enhancements

Potential improvements:
- More sophisticated scoring simulation based on hand types and joker synergies
- Integration with actual Balatro game mechanics for precise calculations
- Machine learning to predict optimal paths to goals
- Save/load goal profiles
- Community seed sharing with goal templates
- Visual timeline of goal achievement
- Warning system for risky plays that might prevent goal achievement

## Technical Details

The GameEngine is implemented in C++ (`game_engine.hpp`) and compiled to WebAssembly via Emscripten. It extends the existing Instance class and reuses the seed-based RNG system to provide deterministic predictions.

Key design principles:
- **Stateless where possible**: Most methods don't modify state unless explicitly stated
- **Flexible input**: Accept game state at any point, not just from start
- **Informative feedback**: Don't just say "no", explain why and suggest alternatives
- **Extensible**: Easy to add new goal types and query methods

## Contributing

To add new features:
1. Add new methods to `GameEngine` struct in `game_engine.hpp`
2. Update `immolate.hpp` bindings to expose to JavaScript
3. Recompile with `em++` (see `build.bat`)
4. Update documentation and examples
5. Test with various seeds and scenarios
