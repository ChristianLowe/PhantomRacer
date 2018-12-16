#pragma once

#include <map>
#include <deque>
#include <chrono>
#include <random>
#include <memory>

#include "strategy.h"

using std::unique_ptr;

static auto rng = std::default_random_engine{};

class Node {
public:
    Board board;
    PieceRange pieceRange;
    Node* parent;

    std::vector<Move> moves;
    std::deque<size_t> moveOrder;
    std::map<Move, unique_ptr<Node>> children;

    u32 winCount = 0;
    u32 totalCount = 0;

    Node(Board gameBoard, PieceRange nodeRange, Node* nodeParent = nullptr)
        : board(gameBoard), pieceRange(nodeRange), parent(nodeParent) {
        moves = gameBoard.getValidMoves(nodeRange);

        for (size_t i = 0; i < moves.size(); i++) {
            moveOrder.push_back(i);
        }

        std::shuffle(moveOrder.begin(), moveOrder.end(), rng);
    }

    inline bool explored() { return moveOrder.empty(); }

    Node* treePolicy() {
        //cout << "Tree Policy" << endl;
        Node* v = this;

        while (!v->moves.empty()) {
            if (v->explored()) {
                //cout << "Explored path" << endl;
                v = v->bestChild().second;
                //cout << "V board:" << endl << v->board << endl;
            } else {
                //cout << "V unexplored." << endl;
                return v->expand();
            }
        }

        return v;
    }

    Node* expand() {
        Move move = moves[moveOrder.back()];
        //cout << "Move idx: " << moveOrder.back() << endl << "Move chosen: " << move << endl;
        moveOrder.pop_back();

        Board boardCopy(board);
        PieceRange oppositeRange;
        if (pieceRange == PieceRange::White) {
            boardCopy.performWhiteMove(move);
            oppositeRange = PieceRange::Black;
        } else {
            boardCopy.performBlackMove(move);
            oppositeRange = PieceRange::White;
        }

        children.insert(std::make_pair(move, unique_ptr<Node>(new Node(boardCopy, oppositeRange))));
        return children[move].get();
    }

    std::pair<Move, Node*> bestChild(double c = 0.7071067811865475) {
        // Default value of c (the exploration constant) is 1/sqrt(2)
        std::pair<Move, Node*> bestPair;
        double bestValue = -1;

        for (auto &pair : children) {
            Node* child = pair.second.get();

            double childValue = child->baseValue();
            childValue += c * sqrt((2 * log(child->totalCount)) / child->totalCount);

            if (childValue > bestValue) {
                bestPair = std::make_pair(pair.first, pair.second.get());
                bestValue = childValue;
            }
        }

        return bestPair;
    }

    GameState defaultPolicy() {
        Board currentBoard(board);
        PieceRange boardRange = pieceRange;

        cout << "Default policy: range " << (int)boardRange << endl;

        while (currentBoard.getGameState() == GameState::IsPlaying) {
            auto currentMoves = currentBoard.getValidMoves(boardRange);
            auto chosenMove = currentMoves[rand() % currentMoves.size()];

            if (boardRange == PieceRange::White) {
                currentBoard.performWhiteMove(chosenMove);
                boardRange = PieceRange::Black;
            } else {
                currentBoard.performBlackMove(chosenMove);
                boardRange = PieceRange::White;
            }
        }

        return currentBoard.getGameState();
    }

    void backpropagation(GameState outcome) {
        Node* currentNode = this;

        while (currentNode) {
            if ((currentNode->pieceRange == PieceRange::White && outcome == GameState::WhiteWins)
            ||  (currentNode->pieceRange == PieceRange::Black && outcome == GameState::BlackWins)) {
                currentNode->winCount++;
            }
            currentNode->totalCount++;
            currentNode = currentNode->parent;
        }
    }

    double inline baseValue() {
        if (totalCount == 0) return 0.0;
        return static_cast<double>(winCount) / totalCount;
    }
};

Move getComputerMove(Board &board, std::vector<Move> &moves) {
    std::chrono::time_point<std::chrono::system_clock> stopTime;
    stopTime = std::chrono::system_clock::now() + std::chrono::seconds(5);
    Node root = Node(board, PieceRange::Black);

    //while (std::chrono::system_clock::now() < stopTime) {
        Node* expandedNode = root.treePolicy();
        //cout << "After TreePolicy: " << endl << expandedNode->board << endl;
        GameState outcome = expandedNode->defaultPolicy();
        expandedNode->backpropagation(outcome);
    //}

    return root.bestChild(0).first;
}
