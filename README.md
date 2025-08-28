# Matrix Comparison and Ranking System

This project implements a matrix-based comparison and ranking system in C++, designed for interactive pairwise comparisons among alternatives. The code has been refactored to adhere to modern C++ standards and incorporates key software design patterns and robust testing.

## Overview

The system maintains a comparison matrix where pairwise preferences between elements (represented as strings) are stored. It employs the Observer, Decorator, Strategy, and Abstract Factory design patterns to ensure modularity, extensibility, and clean separation of concerns.

Users input pairwise comparisons which are recorded in the matrix. The system automatically updates dependent relations via transitive logic, then generates ranked alternatives based on user input and predefined ordinal scales.

## Key Features

- Interactive pairwise comparison with user input prompts and matrix updates.
- Implementation of design patterns:
  - Observer pattern to monitor and react to matrix changes.
  - Decorator pattern to enhance matrix observer functionality with logging.
  - Strategy pattern for flexible comparison logic.
  - Abstract Factory to generate comparators and observers.
- Transitive closure updates in the comparison matrix to maintain consistent relations.
- Automated ranking and sorting of alternatives based on comparison results and external ordinal scales.
- Comprehensive unit tests covering matrix initialization, comparison accuracy, ranking correctness, and transitive relation updates.
- Clean and modular C++ code adhering to modern best practices.

## Technologies
- C++ (Standard libraries)
- Object-oriented design and design patterns
- Console-based user interaction
- Unit testing with custom test framework

## Usage
The program offers two modes: test mode (T) to validate functionality and run unit tests, and run mode (R) where users can perform pairwise comparisons and obtain rankings.

