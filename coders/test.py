def get_burnout_edge(path: str) -> int:
    content: list[str] = []
    with open(path) as f:
        content = f.readlines()
    new_content: list[str] = [line for line in content if line.strip().endswith('is compiling')]
    data: dict[int, list[int]] = {}
    for line in new_content:
        parts: list[str] = line.split()
        timestamp: int = int(parts[0])
        coder_id: int = int(parts[1])
        data.setdefault(coder_id, []).append(timestamp)
    diff_compiles: list[int] = []
    for v in data.values():
        for i in range(1, len(v)):
            diff_compiles.append(v[i] - v[i - 1])
    return max(diff_compiles)


if __name__ == "__main__":
    print(f"Edge burnout: {get_burnout_edge('test.txt')}")
