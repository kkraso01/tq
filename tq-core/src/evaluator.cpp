#include "tq/evaluator.hpp"

namespace tq {

std::vector<Value> Evaluator::apply_step(const Step& step, const Value& current) {
    std::vector<Value> results;
    
    if (step.type == Step::Type::Field) {
        // Access a field in an object
        if (current.is_object()) {
            const Value* field_val = current.get(step.field_name);
            if (field_val) {
                results.push_back(*field_val);
            }
        }
    } else if (step.type == Step::Type::Iterate) {
        // Iterate over array elements
        if (current.is_array()) {
            const auto& arr = current.as_array();
            for (const auto& elem : arr) {
                results.push_back(elem);
            }
        }
    } else if (step.type == Step::Type::Index) {
        // Index into array [n]
        if (current.is_array()) {
            const auto& arr = current.as_array();
            int idx = step.index;
            
            // Handle negative indices
            if (idx < 0) {
                idx = static_cast<int>(arr.size()) + idx;
            }
            
            if (idx >= 0 && idx < static_cast<int>(arr.size())) {
                results.push_back(arr[idx]);
            }
        }
    } else if (step.type == Step::Type::Slice) {
        // Slice array [start:end]
        if (current.is_array()) {
            const auto& arr = current.as_array();
            int size = static_cast<int>(arr.size());
            
            int start = step.slice_start;
            if (start < 0) start = size + start;
            if (start < 0) start = 0;
            if (start > size) start = size;
            
            int end = step.has_slice_end ? step.slice_end : size;
            if (end < 0) end = size + end;
            if (end < 0) end = 0;
            if (end > size) end = size;
            
            for (int i = start; i < end; ++i) {
                results.push_back(arr[i]);
            }
        }
    }
    
    return results;
}

std::vector<Value> Evaluator::evaluate_simple(const std::vector<Step>& steps, const Value& data) {
    if (steps.empty()) {
        return {data};
    }
    
    // Start with the input data as the only current value
    std::vector<Value> current_values = {data};
    
    // Apply each step, fanning out as needed
    for (const auto& step : steps) {
        std::vector<Value> next_values;
        
        // Apply this step to each current value
        for (const auto& val : current_values) {
            auto step_results = apply_step(step, val);
            next_values.insert(next_values.end(), 
                             std::make_move_iterator(step_results.begin()),
                             std::make_move_iterator(step_results.end()));
        }
        
        current_values = std::move(next_values);
        
        // If we got no results, stop early
        if (current_values.empty()) {
            break;
        }
    }
    
    return current_values;
}

std::vector<Value> Evaluator::evaluate(const Query& query, const Value& data) {
    if (query.pipeline.empty()) {
        return {data};
    }
    
    // Start with input data
    std::vector<Value> current_values = {data};
    
    // Process each segment of the pipeline
    for (const auto& segment : query.pipeline) {
        std::vector<Value> next_values;
        
        // Apply this segment to each current value
        for (const auto& val : current_values) {
            auto segment_results = evaluate_simple(segment, val);
            next_values.insert(next_values.end(),
                             std::make_move_iterator(segment_results.begin()),
                             std::make_move_iterator(segment_results.end()));
        }
        
        current_values = std::move(next_values);
        
        if (current_values.empty()) {
            break;
        }
    }
    
    return current_values;
}

} // namespace tq
