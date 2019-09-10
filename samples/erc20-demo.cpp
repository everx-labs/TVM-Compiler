#include "tvm_std.h"
#include "map.h"

class ERC20 {

    typedef int64_t address;

public:

    // sol:
    // constructor(uint256 totalSupply, address owner) public payable {
    //   _totalSupply = totalSupply;
    //   _balances[owner] = totalSupply;
    //   return;
    // }
    ERC20(int64_t totalSupply, address owner) : _totalSupply(totalSupply) {
        _balances[owner] = totalSupply;
    }

    // @dev Total number of tokens in existence

    // sol:
    // function totalSupply() public view returns (uint256) {
    //     return _totalSupply;
    // }
    SPECS int64_t totalSupply() {
        return _totalSupply;
    }

    // sol:
    // function balanceOf(address owner) public view returns (uint256) {
    //     return _balances[owner];
    // }

    // @dev Gets the balance of the specified address.
    // @param owner The address to query the balance of.
    // @return A uint256 representing the amount owned by the passed address.
    SPECS int64_t balanceOf(address owner) {
        return _balances[owner];
    }

    // @dev Function to check the amount of tokens that an owner allowed to a spender.
    // @param owner address The address which owns the funds.
    // @param spender address The address which will spend the funds.
    // @return A uint256 specifying the amount of tokens still available for the spender.

    // sol:
    // function allowance(address owner, address spender) public view returns (uint256) {
    //     return _allowed[owner][spender];
    // }

    SPECS int64_t allowance(address owner, address spender) {
        if (_allowed.has_key(owner)) {
            if (_allowed[owner].has_key(spender)) {
                return _allowed[owner][spender];
            }
        }
        return 0;
    }


    // @dev Transfer token to a specified address
    // @param to The address to transfer to.
    // @param value The amount to be transferred.

    // sol:
    // function transfer(address to, uint256 value, address msg_sender) public returns (bool) {
    //     //_transfer(msg.sender, to, value);
    //     address from = msg_sender;
    //     _balances[from] = _balances[from] - value;
    //     _balances[to] = _balances[to] + value;
    //     return true;
    // }
    SPECS bool transfer(address to, int64_t value, address msg_sender) {
        _balances[msg_sender] -= value;
        _balances[to] += value;
        return true;
    }

    /**
     * @dev Approve the passed address to spend the specified amount of tokens on behalf of msg.sender.
     * Beware that changing an allowance with this method brings the risk that someone may use both the old
     * and the new allowance by unfortunate transaction ordering. One possible solution to mitigate this
     * race condition is to first reduce the spender's allowance to 0 and set the desired value afterwards:
     * https://github.com/ethereum/EIPs/issues/20#issuecomment-263524729
     * @param spender The address which will spend the funds.
     * @param value The amount of tokens to be spent.
     */

    // sol:
    // function approve(address spender, uint256 value, address msg_sender) public returns (bool) {
    //     address owner = msg_sender;
    //     //require(spender != address(0));
    //     //require(owner != address(0));

    //     _allowed[owner][spender] = value;
    //     //_approve(msg.sender, spender, value);
    //     return true;
    // }
    SPECS bool approve(address spender, int64_t value, address msg_sender) {
        _allowed[msg_sender][spender] = value;
        return true;
    }

    /**
     * @dev Transfer tokens from one address to another.
     * Note that while this function emits an Approval event, this is not required as per the specification,
     * and other compliant implementations may not emit the event.
     * @param from address The address which you want to send tokens from
     * @param to address The address which you want to transfer to
     * @param value uint256 the amount of tokens to be transferred
     */

    // sol:
    // function transferFrom(address from, address to, uint256 value, address msg_sender) public returns (bool) {
    //     //_transfer(from, to, value);
    //     _balances[from] = _balances[from] - value;
    //     _balances[to] = _balances[to] + value;
    //     // SafeMath sub -> simple subtraction
    //     //_approve(from, msg.sender, _allowed[from][msg.sender] - value);
    //     _allowed[from][msg_sender] = _allowed[from][msg_sender] - value;
    //     return true;
    // }
    SPECS bool transferFrom(address from, address to, int64_t value, address msg_sender) {
        _balances[from] -= value;
        _balances[to] += value;
        _allowed[from][msg_sender] -= value;
        return true;
    }

private:

    // uint256 private _totalSupply;
    int64_t _totalSupply;

    // mapping (address => uint256) private _balances;
    ton::map<address, int64_t> _balances;

    // mapping (address => mapping (address => uint256)) private _allowed;
    ton::map<address, ton::map<address, int64_t>> _allowed;

};


int main () {
    int64_t owner;
    int64_t spender;
    ERC20 c(42, owner);
    c.allowance(owner, spender);
}
